(function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);var f=new Error("Cannot find module '"+o+"'");throw f.code="MODULE_NOT_FOUND",f}var l=n[o]={exports:{}};t[o][0].call(l.exports,function(e){var n=t[o][1][e];return s(n?n:e)},l,l.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(require,module,exports){
'use strict';

var _dashboard = require('./ui/dashboard.jsx');

var _dashboard2 = _interopRequireDefault(_dashboard);

var _client = require('./client.js');

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var client = new _client.Client('ws://' + self.location.host + '/ws');

var host = document.getElementById('app');
ReactDOM.render(React.createElement(_dashboard2.default, { client: client }), host);

},{"./client.js":2,"./ui/dashboard.jsx":3}],2:[function(require,module,exports){
'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var ParseErrorEvent = exports.ParseErrorEvent = function () {
    function ParseErrorEvent(ex) {
        _classCallCheck(this, ParseErrorEvent);

        this._exception = ex;
    }

    _createClass(ParseErrorEvent, [{
        key: 'exception',
        get: function get() {
            return this._exception;
        }
    }]);

    return ParseErrorEvent;
}();

var MessageEvent = exports.MessageEvent = function () {
    function MessageEvent(data) {
        _classCallCheck(this, MessageEvent);

        this._data = data;
    }

    _createClass(MessageEvent, [{
        key: 'data',
        get: function get() {
            return this._data;
        }
    }]);

    return MessageEvent;
}();

var Client = exports.Client = function () {
    function Client(address) {
        _classCallCheck(this, Client);

        _.extend(this, Backbone.Events);
        this._address = address;
        this.connect();
    }

    _createClass(Client, [{
        key: 'connect',
        value: function connect() {
            var _this = this;

            if (this._ws) {
                this._ws.close();
            }

            var ws = new WebSocket(this._address);

            ws.onopen = function (e) {
                return _this.trigger('open', e);
            };
            ws.onerror = function (e) {
                return _this.trigger('error', e);
            };
            ws.onclose = function (e) {
                return setTimeout(function () {
                    return _this.connect();
                }, 2000);
            };
            ws.onmessage = function (e) {
                try {
                    var data = JSON.parse(e.data);
                    _this.trigger('message', new MessageEvent(data));
                } catch (ex) {
                    _this.trigger('parse-error', new ParseErrorEvent(ex));
                }
            };

            this._ws = ws;
        }
    }]);

    return Client;
}();

},{}],3:[function(require,module,exports){
'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _sensor = require('./sensor.jsx');

var _sensor2 = _interopRequireDefault(_sensor);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var Dashboard = function (_React$Component) {
    _inherits(Dashboard, _React$Component);

    function Dashboard() {
        var _ref;

        var _temp, _this, _ret;

        _classCallCheck(this, Dashboard);

        for (var _len = arguments.length, args = Array(_len), _key = 0; _key < _len; _key++) {
            args[_key] = arguments[_key];
        }

        return _ret = (_temp = (_this = _possibleConstructorReturn(this, (_ref = Dashboard.__proto__ || Object.getPrototypeOf(Dashboard)).call.apply(_ref, [this].concat(args))), _this), _this.state = {
            sensors: {},
            readings: {}
        }, _temp), _possibleConstructorReturn(_this, _ret);
    }

    _createClass(Dashboard, [{
        key: 'componentWillMount',
        value: function componentWillMount() {
            var _this2 = this;

            this.props.client.on('message', function (e) {
                var data = e.data;

                if (data.readings) {
                    data.readings = _.extend(_this2.state.readings, data.readings);
                }

                _this2.setState(data);
            });
        }
    }, {
        key: 'render',
        value: function render() {
            var state = this.state;
            var sensors = _.map(state.readings, function (reading, sensorId) {
                return React.createElement(_sensor2.default, { key: sensorId, reading: reading, label: state.sensors[sensorId] });
            });

            return React.createElement(
                'div',
                { className: 'dashboard' },
                sensors
            );
        }
    }]);

    return Dashboard;
}(React.Component);

exports.default = Dashboard;

},{"./sensor.jsx":4}],4:[function(require,module,exports){
"use strict";

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var formatFloat = function formatFloat(x) {
    var s = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 0;

    var p = Math.pow(10, s);
    x = Math.round(x * p) / p;

    return x.toString();
};

var Sensor = function (_React$Component) {
    _inherits(Sensor, _React$Component);

    function Sensor() {
        _classCallCheck(this, Sensor);

        return _possibleConstructorReturn(this, (Sensor.__proto__ || Object.getPrototypeOf(Sensor)).apply(this, arguments));
    }

    _createClass(Sensor, [{
        key: "render",
        value: function render() {
            var r = this.props.reading;

            return React.createElement(
                "div",
                { className: "element" },
                React.createElement(
                    "div",
                    { className: "header" },
                    this.props.label,
                    React.createElement(
                        "small",
                        null,
                        moment.unix(r.stamp).format('YYYY-MM-DD HH:mm:ss')
                    )
                ),
                React.createElement(
                    "div",
                    { className: "content" },
                    function () {
                        var temp = r.hum_temp || r.bar_pres_temp;

                        if (null !== temp) {
                            return React.createElement(
                                "div",
                                { className: "left" },
                                formatFloat(temp, 1),
                                " ",
                                React.createElement(
                                    "span",
                                    null,
                                    "\xB0C"
                                )
                            );
                        }
                    }(),
                    React.createElement(
                        "div",
                        { className: "right" },
                        function () {
                            if (r.hum_hum) {
                                return React.createElement(
                                    "div",
                                    null,
                                    formatFloat(r.hum_hum, 1),
                                    " ",
                                    React.createElement(
                                        "span",
                                        null,
                                        "%"
                                    )
                                );
                            }
                        }(),
                        function () {
                            if (r.lux) {
                                return React.createElement(
                                    "div",
                                    null,
                                    formatFloat(r.lux, r.lux > 100 ? 0 : 2),
                                    " ",
                                    React.createElement(
                                        "span",
                                        null,
                                        "lx"
                                    )
                                );
                            }
                        }(),
                        function () {
                            if (r.bar_pres_rel) {
                                return React.createElement(
                                    "div",
                                    null,
                                    formatFloat(r.bar_pres_rel, 0),
                                    " ",
                                    React.createElement(
                                        "span",
                                        null,
                                        "\u3371"
                                    )
                                );
                            }
                        }()
                    )
                ),
                React.createElement(
                    "div",
                    { className: "footer" },
                    formatFloat(r.vbat, 2),
                    " V / ",
                    formatFloat(r.vreg, 2),
                    " V"
                )
            );
        }
    }]);

    return Sensor;
}(React.Component);

exports.default = Sensor;

},{}]},{},[1]);
