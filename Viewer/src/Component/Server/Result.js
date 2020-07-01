import React, { Component } from 'react';
import styles from '../Camera/AddStyle.css';



var updateInterval = 1000;
var values = [];
var values2 = [];

export class Result extends Component {
	constructor() {
		super();
		// this.updateChart = this.updateChart.bind(this);
		this.loadData = this.loadData.bind(this);

		this.state = {
            value: [],
       }
       
    }

    componentDidMount() {
		this.loadData();
		setInterval(this.loadData, updateInterval);
		setInterval(this.updateResults, updateInterval);

	}
	loadData() {
		fetch("http://localhost:9000/results")
		.then((response) =>response.json())
		.then(data => {
            this.setData(data);
		});
	}
    
    setData(data) {
        let i;
        let row = [];
        for (var key in data) {
          row.push(this.createData(key,data[key]));
            
        }
        for (i = 0; i < data.length; i++) {
        } 
        this.setState({
            value:row,
        })
      }

    createData(address, count) {
        return {address, count };
    }

    onClickHandler = (e) => {
        const link = e.target.getAttribute('data-item');
        console.log(link);
        window.open(link, 'sharer', 'toolbar=0,status=0,width=548,height=325');
      }

    render() {
      const value = this.state.value;

		return (
            <table className='table'>
                <thead>
                <tr>
                    <th>IP Addresss</th>
                    <th>Movement Count</th>
                </tr>
                </thead>
            <tbody>
                {value.map((item,index) => (
                <tr index={item.address} onClick={this.onClickHandler.bind(this)}>
                    <td data-item={item.address}>{item.address}</td>
                    <td>{item.count}</td>
                </tr>
                ))}
            </tbody>
          </table>
   
		);
	}
}
export default Result;