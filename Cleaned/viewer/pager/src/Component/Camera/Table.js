import React, { Fragment, Component } from 'react';
import { Button, Form, FormGroup, Label, Input, FormText } from 'reactstrap';
import styles from './AddStyle.css';


export class Table extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
            isLoaded: false,
            value: []
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
        let row = [];
        for (var i = 0; i < data.length; i++) {
          row.push(this.createData(data[i][0],data[i][1],data[i][2],data[i][3],data[i][4]));
        } 
        this.setState({
            value:row,
            isLoaded: true
        })
        console.log(row)
      }

    render(){
        const { isLoaded, value} = this.state;

        if (!isLoaded) {
            return <div>Loading...</div>
        }
        else{
            return (
            //     <ul>
            //         {value.map(item => (
            //             <li key={item.id}>
            //             {item.} {item.id}
            //             </li>
            //         ))}
            // </ul>

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
            {value.map(item => (
              <tr key={item.id}>
                <td>{item.id}</td>
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