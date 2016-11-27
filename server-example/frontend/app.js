import Dashboard from './ui/dashboard.jsx';
import {Client} from './client.js';

let client = new Client('ws://' + self.location.host + '/ws');

let host = document.getElementById('app');
let dashboard = ReactDOM.render(React.createElement(Dashboard, {client: client}), host);
