import React, { useRef, useEffect, useState } from 'react';
import { IState, StateManager } from '../../Lib/StateManager';
import { IMessage, ISetRPMMessage, MESSAGE_TYPE } from 'fr-types';
import { CurrentSessionClient } from '../../Streaming';
import { InputEx } from '../../Components';

const CurrentSession = () => {
    const stateManager = useRef(
        new StateManager({
            currentRPM: 0,
            targetRPM: 0,
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
            type: MESSAGE_TYPE.SET_RPM,
            data: {
                targetRPM: Number.parseFloat(targetRPM),
            },
        };
        currentSessionClient.current?.send(message);
    };

    return (
        <div>
            <div>Current State:</div>
            <div>{currentState.state}</div>
            <div>Current Temperature:</div>
            <div>{currentState.temperature}</div>
            <div>Current RPM:</div>
            <div>{currentState.currentRPM}</div>
            <div>Target RPM:</div>
            <div>
                <InputEx
                    onCommitValue={setTargetRPM}
                    initialValue={currentState.targetRPM.toString()}
                />
            </div>
        </div>
    );
};

export default CurrentSession;
