let formatFloat = (x, s = 0) => {
    let p = Math.pow(10, s);
    x = Math.round(x * p) / p;

    return x.toString();
};

export default class Sensor extends React.Component {
    render() {
        let r = this.props.reading;

        return (
            <div className="element">
                <div className="header">
                    {this.props.label}
                    <small>{moment.unix(r.stamp).format('YYYY-MM-DD HH:mm:ss')}</small>
                </div>

                <div className="content">
                    { (() => {
                        let temp = r.hum_temp || r.bar_pres_temp;

                        if(null !== temp) {
                            return (
                                <div className="left">
                                    {formatFloat(temp, 1)} <span>&deg;C</span>
                                </div>
                            );
                        }
                    })() }

                    <div className="right">
                        { (() => {
                            if(r.hum_hum) {
                                return (<div>{formatFloat(r.hum_hum, 1)} <span>%</span></div>);
                            }
                        })() }

                        { (() => {
                            if(r.lux) {
                                return (<div>{formatFloat(r.lux, r.lux > 100 ? 0 : 2)} <span>lx</span></div>);
                            }
                        })() }

                        { (() => {
                            if(r.bar_pres_rel) {
                                return (<div>{formatFloat(r.bar_pres_rel, 0)} <span>㍱</span></div>);
                            }
                        })() }
                    </div>
                </div>

                <div className="footer">
                    {formatFloat(r.vbat, 2)} V / {formatFloat(r.vreg, 2)} V
                </div>
            </div>
        );
    }
}