export interface IState {
    currentRPM: number;
    targetRPM: number;
    temperature: number;
    state: string;
}

export interface IStateUpdateListener {
    identifier: string;
    callback: (newState: IState) => void;
}

export class StateManager {
    private state: IState;
    private updateListeners: Array<IStateUpdateListener>;

    constructor(initialState: IState) {
        this.state = initialState;
        this.updateListeners = [];
    }

    public mutate(updates: Partial<IState>) {
        this.state = { ...this.state, ...updates };
        this.callListeners();
    }

    public getState(): IState {
        return this.state;
    }

    public addStateUpdateListener(listener: IStateUpdateListener): void {
        if (this.updateListeners.find((l) => l.identifier === listener.identifier)) {
            return;
        }

        this.updateListeners.push(listener);
    }

    private callListeners() {
        this.updateListeners.forEach((l) => {
            l.callback(this.state);
        });
    }
}
