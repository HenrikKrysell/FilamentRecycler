import { WebSocket } from 'ws';
import AbstractStreamingClient from './AbstractStreamingClient';
import {
    IStateMessage,
    IMessage,
    ISetRPMMessage,
    IErrorMessage,
    IAckMessage,
    FROM_BACKEND_MESSAGE_TYPE,
    TO_BACKEND_MESSAGE_TYPE,
    IShutdownMessage,
    ISetTargetTemperatureMessage,
} from 'fr-types';
import { IStreamClient, IStreamingClientProperties } from '../../Interfaces/Streaming.interface';
import { EventEmitter } from 'events';
import {
    FromControllerMessageType,
    IControllerSetRPMMessage,
    IControllerSetTargetTemperatureMessage,
    IControllerShutdownMessage,
    IControllerState,
    IControllerStateMessage,
    ToControllerMessageType,
} from '../../Controller';

class CurrentSessionClient extends AbstractStreamingClient implements IStreamClient {
    private controllerEventEmitter: EventEmitter;

    constructor(
        socket: WebSocket,
        properties: IStreamingClientProperties,
        controllerEventEmitter: EventEmitter
    ) {
        super(socket, properties);

        this.controllerEventEmitter = controllerEventEmitter;
        this.controllerEventEmitter.on(
            FromControllerMessageType.STATE_UPDATED,
            (msg: IControllerState) => {
                this.send<IStateMessage>({
                    type: FROM_BACKEND_MESSAGE_TYPE.STATE_UPDATED,
                    data: {
                        currentRPM: msg.currentRPM,
                        targetRPM: msg.targetRPM,
                        temperature: msg.temperature,
                        state: msg.state,
                    },
                });
            }
        );
        // listen to event emitter here to update the state!
    }

    onMessage(message: IMessage<unknown>): void {
        console.log('onMessage', message);
        switch (message.type) {
            case TO_BACKEND_MESSAGE_TYPE.SET_RPM:
                const setTargetRpmMessage = message as IMessage<ISetRPMMessage>;
                this.handleSetTargetRPM(setTargetRpmMessage.data);
                break;
            case TO_BACKEND_MESSAGE_TYPE.SET_TARGET_TEMPERATURE:
                const setTargetTemperatureMessage =
                    message as IMessage<ISetTargetTemperatureMessage>;
                this.handleSetTargetTemperature(setTargetTemperatureMessage.data);
                break;
            case TO_BACKEND_MESSAGE_TYPE.SHUTDOWN:
                const shutdownMessage = message as IMessage<IShutdownMessage>;
                this.handleShutdown(shutdownMessage.data);
                break;
        }
    }

    handleShutdown(message: IShutdownMessage): void {
        const event: IControllerShutdownMessage = {};
        this.controllerEventEmitter.emit(ToControllerMessageType.SHUTDOWN, event);

        this.send({
            type: FROM_BACKEND_MESSAGE_TYPE.ACK,
            data: {
                success: true,
            },
        });
    }

    handleSetTargetTemperature(message: ISetTargetTemperatureMessage): void {
        const event: IControllerSetTargetTemperatureMessage = {
            targetTemperature: message.targetTemperature,
        };
        this.controllerEventEmitter.emit(ToControllerMessageType.SET_TARGET_TEMPERATURE, event);

        this.send({
            type: FROM_BACKEND_MESSAGE_TYPE.ACK,
            data: {
                success: true,
            },
        });
    }

    handleSetTargetRPM(message: ISetRPMMessage): void {
        const event: IControllerSetRPMMessage = { targetRPM: message.targetRPM };
        this.controllerEventEmitter.emit(ToControllerMessageType.SET_TARGET_RPM, event);

        this.send({
            type: FROM_BACKEND_MESSAGE_TYPE.ACK,
            data: {
                success: true,
            },
        });
    }
}

export default CurrentSessionClient;
