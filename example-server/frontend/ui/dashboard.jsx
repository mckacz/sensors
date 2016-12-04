import Sensor from './sensor.jsx';

export default class Dashboard extends React.Component {
    state = {
        sensors: {},
        readings: {}
    };

    componentWillMount() {
        this.props.client.on('message', e => {
            let data = e.data;

            if(data.readings) {
                data.readings = _.extend(this.state.readings, data.readings);
            }

            this.setState(data);
        });
    }

    render() {
        let state = this.state;
        let sensors = _.map(
            state.readings,
            (reading, sensorId) => <Sensor key={sensorId} reading={reading} label={state.sensors[sensorId]} />
        );

        return (
            <div className="dashboard">
                {sensors}
            </div>
        );
    }
}
