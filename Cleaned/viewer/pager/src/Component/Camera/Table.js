import React, { Fragment, Component } from 'react';
import { Button, Form, FormGroup, Label, Input, FormText } from 'reactstrap';
import styles from './AddStyle.css';
import Rtsp from '../Rtsp';
import { Redirect } from 'react-router';
import { Router } from 'react-router';

export class Table extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
            isLoaded: false,
            value: [],
            ip: ''
        }        
    }

    componentDidMount() {
        fetch('http://localhost:9000/data')
            .then(response => response.json())
            .then(data => {
                this.setData(data);
            });
    }

    createData(id, ip, addedDate, status,  additional) {
        return { id, ip, addedDate, status, additional };
    }
    
    setData(data) {
        let i;
        let row = [];
        for (i = 0; i < data.length; i++) {
          row.push(this.createData(data[i][0],data[i][1],data[i][2],data[i][3],data[i][4]));
        } 
        this.setState({
            value:row,
            isLoaded: true,
        })
        this.props.changeLength(row.length);
        console.log(row)
      }

    onClickHandler = (e) => {
      const ip = e.target.getAttribute('data-item');
      const index = e.target.getAttribute('index');
      console.log(ip);
      this.setState({
        ip: ip
       });
    }

    render(){
      const { isLoaded, value} = this.state;
      if (this.state.ip) {
        return <Rtsp
                ip = {this.state.ip}
                ></Rtsp>
      } 
        

      else if (!isLoaded) {
            return <div>Loading...</div>
        }
      else{
            return (
            <table className='table'>
              <thead>
                <tr>
                  <th>Camera ID</th>
                  <th>IP Addresss</th>
                  <th>Status</th>
                  <th>Added Date</th>
                  <th>Additional Details</th>
                </tr>
              </thead>
              <tbody>
              {value.map((item,index) => (
                <tr index={item.id} onClick={this.onClickHandler.bind(this)}>
                  <td data-item={item.ip} >{item.id}</td>
                  <td>{item.ip}</td>
                  <td>{item.status}</td>
                  <td>{item.addedDate}</td>
                  <td>{item.additional}</td>
                </tr>
              ))}
              </tbody>
            </table>
     
            );
        }
    }
  
}

export default Table;