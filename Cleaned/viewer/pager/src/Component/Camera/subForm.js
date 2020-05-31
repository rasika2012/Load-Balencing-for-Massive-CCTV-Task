import React, { Fragment, Component } from 'react';
import { Button, Form, FormGroup, Label, Input, FormText } from 'reactstrap';


export class SubForm extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
             ip : props.IPAddress,
             comments: '',
        }        
    }

    handleCommentsChange = (event) => {
        this.setState({
            comments: event.target.value
        })
    
    }

    render(){
        return (
            <Fragment>
              <Form className='formAdditional' onSubmit={this.handleSubmit}>
                  <h3>Additional Details</h3>
      
                  <FormGroup>
                      <Label for="id">Camera ID</Label>
                      <Input type="text" name="id" id="id" placeholder="ID" value="ID0088" readOnly />
                  </FormGroup>
                  <FormGroup>
                      <Label for="IPaddress">IP Address</Label>
                      <Input type="text" name="ip" id="ip" placeholder="IP Address" value={this.state.ip} readOnly />
                  </FormGroup>
                  <FormGroup>
                      <Label for="Details">Additional Comments</Label>
                      <Input type="textarea" name="text" value={this.state.comments} onChange={this.handleCommentsChange} id="comments" />
                  </FormGroup>
                  <Button>Submit</Button>
                  </Form>
                

              </Fragment>
        );
    }
  
}

export default SubForm;