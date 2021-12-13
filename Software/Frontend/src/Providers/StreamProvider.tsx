import React, { createContext } from 'react';
import { IStreamingClient } from '../Interfaces/General.interface';
import { IStreamContext } from '../Interfaces/Providers.interface';

export const StreamContext = createContext<IStreamContext>({
    stream: undefined,
});

interface IStreamProvider {
    children: React.ReactElement;
    stream?: IStreamingClient;
}

const StreamProvider = ({ children, stream }: IStreamProvider) => {
    return <StreamContext.Provider value={{ stream }}>{children}</StreamContext.Provider>;
};

export default StreamProvider;
