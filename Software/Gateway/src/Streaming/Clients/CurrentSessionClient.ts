import { WebSocket } from 'ws';
import AbstractStreamingClient from './AbstractStreamingClient';
import {
    IStateMessage,
    IMessage,
    ISetRPMMessage,
    IErrorMessage,
    IAckMessage,
    MESSAGE_TYPE,
} from 'fr-types';
import { IStreamClient, IStreamingClientProperties } from '../../Interfaces/Streaming.interface';
import { EventEmitter } from 'events';
import {
    ControllerMessageType,
    IControllerSetRPMMessage,
    IControllerStateMessage,
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
            ControllerMessageType.STATE_UPDATED,
            (msg: IControllerStateMessage) => {
                this.send({
                    type: MESSAGE_TYPE.STATE_UPDATED,
                    data: {
                        currentRPM: msg.stateUpdate.currentRPM,
                        targetRPM: msg.stateUpdate.targetRPM,
                    },
                });
            }
        );
        // listen to event emitter here to update the state!
    }

    onMessage(message: IMessage<unknown>): void {
        console.log('onMessage', message);
        switch (message.type) {
            case MESSAGE_TYPE.SET_RPM:
                const setTargetRpmMessage = message as IMessage<ISetRPMMessage>;
                this.handleSetTargetRPM(setTargetRpmMessage.data);
                break;
        }
    }

    handleSetTargetRPM(message: ISetRPMMessage): void {
        console.log('SetRPM');
        const event: IControllerSetRPMMessage = { targetRPM: message.targetRPM };
        this.controllerEventEmitter.emit(ControllerMessageType.SET_RPM, event);

        this.send({
            type: MESSAGE_TYPE.ACK,
            data: {
                success: true,
            },
        });
    }
}

export default CurrentSessionClient;
