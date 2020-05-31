import React from 'react';
import { withStyles, makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';
import styles from './AddStyle.css'
import Button from '@material-ui/core/Button';


const StyledTableCell = withStyles((theme) => ({
  head: {
    backgroundColor: theme.palette.common.black,
    color: theme.palette.common.white,
  },
  body: {
    fontSize: 14,
  },
}))(TableCell);

const StyledTableRow = withStyles((theme) => ({
  root: {
    '&:nth-of-type(odd)': {
      backgroundColor: theme.palette.action.hover,
    },
  },
}))(TableRow);

function createData(id, ip, status, addedDate, additional, add) {
  return { id, ip, status, addedDate, additional, add };
}

const rows = [
  createData('CAM0001', '192.168.101.1', 'running', '2018/12/25'),
  createData('CAM0002', '192.168.101.2', 'running', '2018/12/25'),
  createData('CAM0003', '192.168.101.3', 'running', '2018/12/25', 'type = Infrared/Night Vision CCTV Camera'),
  createData('CAM0004', '192.168.101.1', 'running', '2018/12/25'),
  createData('CAM0005', '192.168.101.10', 'stopped', '2018/12/26', 'type = Infrared/Night Vision CCTV Camera'),
];

const useStyles = makeStyles({
  table: {
    maxWidth: 900,
    marginTop: 20,
    margin: 'auto',

    
  },
});
function more(event){
    const val = event.target.getAttribute('data-item');
    const key = event.target.getAttribute('key');
    console.log(rows[val]);
}
function getData() {
  fetch('http://localhost:9000/data')
      .then(response => response.json())
      .then(data => {
        setData(data);
        
      });
}

const row = [
  createData('CAM0001', '192.168.101.1', 'running', '2018/12/25')
];

 function setData(data) {
  for (var i = 0; i < data.length; i++) {
    // row.push(createData(data[i][0],data[i][1],data[i][2],data[i][3],data[i][4],data[i][5]));
  } 
  row.push(createData('CAM0001', '192.168.101.1', 'running', '2018/12/25'));
  console.log(row)
}

function check() {
    console.log(row)
}


export default function CustomizedTables() {
   
  const classes = useStyles();
  
  
  return (
    <TableContainer component={Paper}>
      <Table className={classes.table} aria-label="customized table">
        <TableHead>
          <TableRow>
            <StyledTableCell>Camera ID</StyledTableCell>
            <StyledTableCell align="right">IP</StyledTableCell>
            <StyledTableCell align="right">Status</StyledTableCell>
            <StyledTableCell align="right">Established date</StyledTableCell>
            <StyledTableCell align="center">Additional Details</StyledTableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {row.map((row,index) => (
            <StyledTableRow key={row.name}>
              {/* <StyledTableCell component="th" scope="row">
                {row.name}
              </StyledTableCell> */}
              <StyledTableCell key={row.id} data-item={index} onClick={more} align="right">{row.id}</StyledTableCell>
              <StyledTableCell align="right">{row.ip}</StyledTableCell>
              <StyledTableCell align="right">{row.status}</StyledTableCell>
              <StyledTableCell align="right">{row.addedDate}</StyledTableCell>
              <StyledTableCell align="right">{row.additional}</StyledTableCell>
            </StyledTableRow>
          ))}
        </TableBody>
      </Table>
   
      <Button  variant="contained" color="primary" onClick={check}>
      Add Camera
      </Button>
      </TableContainer>
);
}
