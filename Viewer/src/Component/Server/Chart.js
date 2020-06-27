import React, { Component } from 'react';
import CanvasJSReact from './../assets/canvasjs.react';
var CanvasJSChart = CanvasJSReact.CanvasJSChart;

// var dps = [{x: 1, y: 10}, {x: 2, y: 13}, {x: 3, y: 18}, {x: 4, y: 20}, {x: 5, y: 17},{x: 6, y: 10}, {x: 7, y: 13}, {x: 8, y: 18}, {x: 9, y: 20}, {x: 10, y: 17}];   //dataPoints.
// var dps2 = [{x: 1, y: 13}, {x: 2, y: 18}, {x: 3, y: 20}, {x: 4, y: 17},{x: 5, y: 10}, {x: 6, y: 13}, {x: 8, y: 18}, {x: 9, y: 20}, {x: 10, y: 17}];   //dataPoints.
var dps = [];
var dps2 = [];
var xVal = dps.length + 1;
var yVal = 15;
var yVal2 = 15;
var updateInterval = 1000;


export class Chart extends Component {
	constructor() {
		super();
		this.updateChart = this.updateChart.bind(this);
		this.loadData = this.loadData.bind(this);

        this.state = {
			serverTime1 : '',
			serverTime2 : '',
			serverCount1: '',
			serverCount2: ''
	   }
	}
	componentDidMount() {
		this.loadData();
		setInterval(this.loadData,1000);

		setInterval(this.updateChart, updateInterval);
	}
	loadData() {
		fetch("http://localhost:9000/timing")
		.then((response) => response.json())
		.then(data => {
			console.log(data);
			this.setState({
				// serverTime1 : data.ser1.time,
				serverTime2 : data.ser2.time,
				// serverCount1: data.ser1.count,
				serverCount2: data.ser2.count
			})
		});
	}

	updateChart() {
		if (this.state.serverCount1 != 0 ){
			yVal = this.state.serverTime1;
		} 
		else {
			yVal = 0;
		}
		dps.push({x: xVal,y: yVal});
		if (this.state.serverCount2 != 0) {
			yVal2 = this.state.serverTime2
		}
		else {
			yVal2 = 0;
		}
		dps2.push({x: xVal,y: yVal2});
		
		xVal++;
		if (dps.length >  10 ) {
			dps.shift();
			dps2.shift();
		}
		this.chart.render();
	}
	render() {
		const options = {
			title :{
				text: "Server Processing time"
			},
			data: [{
				type: "line",
				dataPoints : dps
			},
			{
				type: "line",
				dataPoints : dps2
			}]
		}
		return (
		<div>
			<CanvasJSChart options = {options}
				 onRef={ref => this.chart = ref}
			/>
		</div>
		);
	}
}
export default Chart;