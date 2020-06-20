import React, { Component } from 'react'
import {
    Container, Col, Form,
    FormGroup, Label, Input,
  } from 'reactstrap';
import Button from '@material-ui/core/Button';
import TextField from '@material-ui/core/TextField';

import styles from './AddStyle.css';
import SubForm from './subForm';
import Table from './Table';


export class Add extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
             id : '',
             ip : '',
             other: '',
             isSubmitted: false,
             length: ''
            //  values: []
        }
             
    }
    
    handleIdChange = (event) => {
        this.setState({
            id: event.target.value
        })
    
    }
    handleIPChange = (event) => {
        this.setState({
            ip: event.target.value
        })
    
    }
    handleOtherChange = (event) => {
        this.setState({
            other: event.target.value
        })
    
    }

    handleSubmit = event => {
        if ( this.state.ip) {
            this.setState({isSubmitted: true});
        }
        //  alert(`${this.state.id} ${this.state.ip}`)
         event.preventDefault()
    }

    submitForm(e) {
        e.preventDefault();
    }

    check() {
        console.log(this.state.values);
    }

    changeLength(length) {
        this.setState({
          length: length
        });
        console.log("c",this.state.length)
    }

    
 
    render() {
        if (this.state.isSubmitted) {
            return  <SubForm 
                        IPAddress = {this.state.ip}
                        length = {this.state.length}>                           
                    </SubForm>
        }

        return (
            <React.Fragment>
            <form className = 'form' onSubmit={this.handleSubmit}>
                <div>
                    Form
                </div>
                <div>    
                    <TextField className='add' id="standard-basic" label="Camera IP" type='text' value={this.state.ip} onChange={this.handleIPChange} />
                </div>
                <Button  type= 'submit' variant="contained" color="primary">
                    Add Camera
                </Button> 
            </form>    
            {/* <SubForm></SubForm>  */}

            <Table  
                length = {this.state.length}   
                changeLength = {this.changeLength.bind(this)}>
            </Table>
        </React.Fragment>
        )
    }


}


export default Add
