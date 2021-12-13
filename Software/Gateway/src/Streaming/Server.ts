import { WebSocketServer, WebSocket } from 'ws';
import { createServer, IncomingMessage } from 'http';
import {
    IStreamingClientProperties,
    IStreamingServerOptions,
} from '../Interfaces/Streaming.interface';
import { STREAMING_CONTEXT } from 'fr-types';

class StreamingServer {
    constructor(options: IStreamingServerOptions) {
        const server = createServer();
        const wss = new WebSocketServer({ noServer: true });
        wss.on(
            'connection',
            (ws: WebSocket, request: IncomingMessage, client: IStreamingClientProperties) => {
                options.contextHandler(ws, client);
            }
        );

        server.on('upgrade', function upgrade(request: IncomingMessage, socket: any, head: Buffer) {
            if (!request.headers['sec-websocket-protocol']) {
                socket.write('HTTP/1.1 401 Unauthorized\r\n\r\n');
                socket.destroy();
                return;
            }
            const headers: Array<string> = request.headers['sec-websocket-protocol'].split(', ');
            const objectHeaders: Record<string, string> = {};
            for (let i = 0; i < headers?.length; i += 2) {
                const key = headers[i];
                const value = headers[i + 1];
                objectHeaders[key] = value;
            }

            const client: IStreamingClientProperties = {
                context: objectHeaders['context'] as STREAMING_CONTEXT,
                protocols: objectHeaders,
            };
            try {
                wss.handleUpgrade(request, socket, head, (ws) => {
                    wss.emit('connection', ws, request, client);
                });
            } catch (_) {
                socket.write('HTTP/1.1 401 Unauthorized\r\n\r\n');
                socket.destroy();
            }
        });

        server.listen(options.port, () => {
            console.log('Streaming server started');
        });
    }
}

export default StreamingServer;
