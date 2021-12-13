import { CurrentSessionClient, StreamingServer, ConsoleClient } from './Streaming';
import { EventEmitter } from 'events';
import { STREAMING_CONTEXT } from 'fr-types';
import { ControllerClient } from './Controller/ControllerClient';
import ConsoleLog from './Controller/ConsoleLog';
// https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow&t=Filament%20Recycler%0A-----------------------
const banner = `
███████╗██╗██╗      █████╗ ███╗   ███╗███████╗███╗   ██╗████████╗    ██████╗ ███████╗ ██████╗██╗   ██╗ ██████╗██╗     ███████╗██████╗     
██╔════╝██║██║     ██╔══██╗████╗ ████║██╔════╝████╗  ██║╚══██╔══╝    ██╔══██╗██╔════╝██╔════╝╚██╗ ██╔╝██╔════╝██║     ██╔════╝██╔══██╗    
█████╗  ██║██║     ███████║██╔████╔██║█████╗  ██╔██╗ ██║   ██║       ██████╔╝█████╗  ██║      ╚████╔╝ ██║     ██║     █████╗  ██████╔╝    
██╔══╝  ██║██║     ██╔══██║██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║       ██╔══██╗██╔══╝  ██║       ╚██╔╝  ██║     ██║     ██╔══╝  ██╔══██╗    
██║     ██║███████╗██║  ██║██║ ╚═╝ ██║███████╗██║ ╚████║   ██║       ██║  ██║███████╗╚██████╗   ██║   ╚██████╗███████╗███████╗██║  ██║    
╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝       ╚═╝  ╚═╝╚══════╝ ╚═════╝   ╚═╝    ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝    
                                                                                                                                          
                                                                                                                                          
                                                                                                                                          
█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗█████╗
╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝╚════╝
`;

process.on('unhandledRejection', (up) => {
    throw up;
});

const run = () => {
    const controllerEventEmitter = new EventEmitter();
    const consoleLog = new ConsoleLog(controllerEventEmitter, 1000);
    const controllerClient = new ControllerClient(5002, controllerEventEmitter);

    new StreamingServer({
        port: process.env.PORT ? Number.parseInt(process.env.PORT) : 3000,
        contextHandler: (ws: any, properties: any) => {
            if (properties.context === STREAMING_CONTEXT.CURRENT_SESSION) {
                return new CurrentSessionClient(ws, properties, controllerEventEmitter);
            }
            if (properties.context === STREAMING_CONTEXT.CONSOLE) {
                return new ConsoleClient(ws, properties, controllerEventEmitter, consoleLog);
            }
            // if (properties.context === 'recordedSession') {
            //     return new ProcessDebuggingClient(ws, properties);
            // }
            throw new Error('Unsupported context');
        },
    });

    controllerClient.Connect();

    console.log(banner);
};

run();
