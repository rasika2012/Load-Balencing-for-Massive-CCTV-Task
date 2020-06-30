import React, { Component } from 'react';
import CanvasJSReact from '../assets/canvasjs.react';
var CanvasJSChart = CanvasJSReact.CanvasJSChart;

// var dps = [{x: 1, y: 10}, {x: 2, y: 13}, {x: 3, y: 18}, {x: 4, y: 20}, {x: 5, y: 17},{x: 6, y: 10}, {x: 7, y: 13}, {x: 8, y: 18}, {x: 9, y: 20}, {x: 10, y: 17}];   //dataPoints.
// var dps2 = [{x: 1, y: 13}, {x: 2, y: 18}, {x: 3, y: 20}, {x: 4, y: 17},{x: 5, y: 10}, {x: 6, y: 13}, {x: 8, y: 18}, {x: 9, y: 20}, {x: 10, y: 17}];   //dataPoints.
// var dps = [];
var xVal = 1;
var yVal = [];
var valuesArr = [];
var updateInterval = 1000;
var data = [];

export class Char extends Component {
	constructor() {
		super();
		this.updateChart = this.updateChart.bind(this);
		this.loadData = this.loadData.bind(this);

		this.state = {
			data: [],
			length: '',
			serverTime : [],
			serverCount: [],
	   }
	}
	componentDidMount() {
		this.loadData();
		setInterval(this.loadData,1000);

		setInterval(this.updateChart, updateInterval);
		setInterval(this.updateIndivChart, updateInterval);

	}
	loadData() {
		fetch("http://localhost:9000/timing")
		.then((response) =>response.json())
		.then(data => {
			this.setState({
				data: data,
				length:Object.keys(data).length
			})
		});
	}

	updateChart() {
		
		for (var i=1; i <= this.state.length; i++) {
			if(yVal[i-1] == undefined){
				yVal[i-1] = [];
			}
			// console.log(this.state.data['ser'+i]);

            if (this.state.data['ser'+i].count != 0 ){
				// yVal[i-1].push(this.state.data['ser'+i].time);
				yVal[i-1].push(1000 + Math.random() * 1000);
            } 
            else {
				// yVal[i-1].push(this.state.data['ser'+i].time);
				yVal[i-1].push(1000 + Math.random() * 1000);

            }
		}

		data = [];
		yVal.forEach(y => {
			let dps = [];
			xVal = 0;
			y.forEach(yInstance => {				
				if (dps.length > 10){
					dps.shift();
				}
				dps.push({x: xVal++,y: yInstance})
			});
			
			data.push({
				type: "line",
				dataPoints: dps
			})
				
		});
		this.chart.render();
	}

	render() {
		const options = {
			title :{
				text: "Server Processing time"
			},
			data: data,
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
export default Char;