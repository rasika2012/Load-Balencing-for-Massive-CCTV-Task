import React, { Fragment, Component } from 'react';
import { Button, Form, FormGroup, Label, Input, FormText } from 'reactstrap';


export class SubForm extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
             ip : props.IPAddress,
             length: Number(props.length)+1,
             comments: '',
        }        
    }

    handleCommentsChange = (event) => {
        this.setState({
            comments: event.target.value
        })
    }    
    handleSubmit = (e) => {
        // e.preventDefault();
        // console.log(this.state.comments)
        const requestOptions = {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ 
                                id: this.state.length,
                                ip: this.state.ip,
                                comments: this.state.comments
                            })
        };
        fetch('http://localhost:9000/submitData', requestOptions)
            .then(response => response.json())
            .then(data => console.log(data));
    }
    

    render(){
        return (
            <Fragment>
              <Form className='formAdditional' onSubmit={this.handleSubmit}>
                  <h3>Additional Details</h3>
      
                  <FormGroup>
                      <Label for="id">Camera ID</Label>
                      <Input type="text" name="id" id="id" placeholder="ID" value={"CAM0"+this.state.length} readOnly />
                  </FormGroup>
                  <FormGroup>
                      <Label for="IPaddress">IP Address</Label>
                      <Input type="text" name="ip" id="ip" placeholder="IP Address" value={this.state.ip} readOnly />
                  </FormGroup>
                  <FormGroup>
                      <Label for="Details">Additional Comments</Label>
                      <Input type="textarea" name="text" value={this.state.comments} onChange={this.handleCommentsChange} id="comments" />
                  </FormGroup>
                  <Button type='submit'>Submit</Button>
                  </Form>
                

              </Fragment>
        );
    }
  
}

export default SubForm;