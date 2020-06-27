import React from "react";
import { Chart } from "react-charts";

import { makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';


export function Timing(props) {
  const [newDataSet, setNewDataSet] = React.useState([]);
  const [triger, setTriger] = React.useState(0);
  const [newData, setNewData] = React.useState(0);
  React.useEffect(() => {
    timeSensativeAction();
  }, [triger]);

  const sleep = (milliseconds) => {
    return new Promise((resolve) => setTimeout(resolve, milliseconds));
  };

  async function timeSensativeAction() {
    //must be async func
    await sleep(1000); //wait 5 seconds

    setTriger(triger + 1);
   

    fetch("http://"+props.ip+"/timing")
      .then((response) => response.json())
      .then((data) => {
        Object.keys(data).forEach(updates);
        setNewData(data);
        // console.log(data['ser1'])
      });
  }

  var data = [] 

  async function updates(item, index) {
	if(index==0){
		
		data=[]
	}
	data.push({ name : item, time:newData[item]?.time ,count: newData[item]?.count})
	await setNewDataSet(data)
    
  }

  const lineChart = (
    // A react-chart hyper-responsively and continuusly fills the available
    // space of its parent element automatically
    <div
    >
     
	  <TableContainer component={Paper}>
      <Table  size="small" aria-label="a dense table">
        <TableHead>
          <TableRow>
            <TableCell>Server</TableCell>
            <TableCell align="right">Time</TableCell>
            <TableCell align="right">Count</TableCell>
          </TableRow>
        </TableHead>
        
		<TableBody>
          {newDataSet.map((row) => (
            <TableRow key={row.name}>
              <TableCell component="th" scope="row">
                {row.name}
              </TableCell>
              <TableCell align="right">{row.time}</TableCell>
              <TableCell align="right">{row.count}</TableCell>
             
            </TableRow>
          ))}
        </TableBody>
		
      </Table>
    </TableContainer>

    </div>
  );

  return lineChart;
}
