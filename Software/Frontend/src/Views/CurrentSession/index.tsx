import React, { useRef, useEffect, useState } from 'react';
import { IState, StateManager } from '../../Lib/StateManager';
import {
    IMessage,
    ISetRPMMessage,
    FROM_BACKEND_MESSAGE_TYPE,
    TO_BACKEND_MESSAGE_TYPE,
    ISetTargetTemperatureMessage,
    IShutdownMessage,
} from 'fr-types';
import { CurrentSessionClient } from '../../Streaming';
import { InputEx } from '../../Components';

const CurrentSession = () => {
    const stateManager = useRef(
        new StateManager({
            currentRPM: 0,
            targetRPM: 0,
            targetTemperature: 0,
            temperature: 0,
            state: 'None',
        })
    );
    const [currentState, setCurrentState] = useState(stateManager.current.getState());

    const createNewStreamingClient = (): CurrentSessionClient => {
        const client = new CurrentSessionClient(stateManager.current);

        const clientSignals: Array<{ nodeId: string; value: number; timestamp: number }> = [];

        // client.onSignalValueChange((nodeId: string, value: number) => {
        //     // clientSignals.push({ nodeId, value, timestamp: new Date().getTime() });
        //     // setSignalRecord(clientSignals);
        //     // stateManager.current.mutate({
        //     //     apa = 'dfdsfsd',
        //     // });
        // });
        client.onConnection(() => {});

        return client;
    };
    const currentSessionClient = useRef<CurrentSessionClient | undefined>();

    useEffect(() => {
        currentSessionClient.current = createNewStreamingClient();

        stateManager.current.addStateUpdateListener({
            identifier: 'current-session',
            callback: (newState: IState) => {
                setCurrentState(newState);
            },
        });
    }, []);

    const setTargetRPM = (targetRPM: string) => {
        const message: IMessage<ISetRPMMessage> = {
            type: TO_BACKEND_MESSAGE_TYPE.SET_RPM,
            data: {
                targetRPM: Number.parseFloat(targetRPM),
            },
        };
        currentSessionClient.current?.send(message);
    };

    const setTargetTemperature = (targetTemperature: string) => {
        const message: IMessage<ISetTargetTemperatureMessage> = {
            type: TO_BACKEND_MESSAGE_TYPE.SET_TARGET_TEMPERATURE,
            data: {
                targetTemperature: Number.parseFloat(targetTemperature),
            },
        };
        currentSessionClient.current?.send(message);
    };

    const shutdownServer = () => {
        const message: IMessage<IShutdownMessage> = {
            type: TO_BACKEND_MESSAGE_TYPE.SHUTDOWN,
            data: {},
        };
        currentSessionClient.current?.send(message);
    };

    return (
        <div>
            <div>Current State:</div>
            <div>{currentState.state}</div>
            <div>Current Temperature:</div>
            <div>{currentState.temperature}</div>
            <div>Target Temperature:</div>
            <div>
                <InputEx
                    onCommitValue={setTargetTemperature}
                    initialValue={currentState.targetTemperature.toString()}
                />
            </div>
            <div>Current RPM:</div>
            <div>{currentState.currentRPM}</div>
            <div>Target RPM:</div>
            <div>
                <InputEx
                    onCommitValue={setTargetRPM}
                    initialValue={currentState.targetRPM.toString()}
                />
            </div>
            <button onClick={shutdownServer}>Shutdown server</button>
        </div>
    );
};

export default CurrentSession;
