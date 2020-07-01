import React from 'react';
import { withStyles, makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';
import styles from './../Camera/AddStyle.css';

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

function createData(id, ip, status, addedDate, additional) {
  return { id, ip, status, addedDate, additional };
}

const rows = [
  createData('MAINSERVER', '35.229.166.153', 'running', '2020/06/20'),
  createData('SERVER01', '35.226.213.127', 'running', '2020/06/20'),
  createData('SERVER02', '34.101.225.44', 'running', '2020/06/20'),
  createData('SERVER03', '35.192.57.63', 'running', '2020/06/21'),
  createData('SERVER03', '35.224.119.100', 'running', '2020/06/21'),
  createData('SERVER04', '35.223.149.101', 'running', '2020/06/21'),
  createData('SERVER05', '104.155.222.42', 'running', '2020/06/21'),
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

export default function CustomizedTables() {
  const classes = useStyles();

  
  return (
    <TableContainer component={Paper}>
      <Table className={classes.table} aria-label="customized table">
        <TableHead>
          <TableRow>
            <StyledTableCell>Server ID</StyledTableCell>
            <StyledTableCell align="right">IP</StyledTableCell>
            <StyledTableCell align="right">Status</StyledTableCell>
            <StyledTableCell align="right">Established date</StyledTableCell>
            <StyledTableCell align="center">Additional Details</StyledTableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {rows.map((row,index) => (
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
    </TableContainer>
 );
}
