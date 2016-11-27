export class ParseErrorEvent {
    constructor(ex) {
        this._exception = ex;
    }

    get exception() {
        return this._exception;
    }
}

export class MessageEvent {
    constructor(data) {
        this._data = data;
    }

    get data() {
        return this._data;
    }
}

export class Client {
    constructor(address) {
        _.extend(this, Backbone.Events);
        this._address = address;
        this.connect();
    }

    connect() {
        if(this._ws) {
            this._ws.close();
        }

        let ws = new WebSocket(this._address);

        ws.onopen = e => this.trigger('open', e);
        ws.onerror = e => this.trigger('error', e);
        ws.onclose = e => setTimeout(() => this.connect(), 2000);
        ws.onmessage = e => {
            try {
                let data = JSON.parse(e.data);
                this.trigger('message', new MessageEvent(data));
            } catch(ex) {
                this.trigger('parse-error', new ParseErrorEvent(ex));
            }

        };

        this._ws = ws;
    }
}