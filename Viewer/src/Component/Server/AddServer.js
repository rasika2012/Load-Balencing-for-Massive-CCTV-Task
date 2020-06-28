import React, { Component } from 'react'
import Button from '@material-ui/core/Button';
import TextField from '@material-ui/core/TextField';
import Tables from './Tables';
import Chart from './Chart'



export class Add extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
             id : '',
             ip : '',
             other: '',
             isSubmitted: false
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


 
    render() {
        // if (this.state.isSubmitted) {
        //     // return <AdditionalDetails ip={this.state.ip}/>;
        //     return  <SubForm IPAddress ={this.state.ip}></SubForm>

        //   }

        return (
            <React.Fragment>
            <form className = 'form' onSubmit={this.handleSubmit}>
                <div>
                    Form
                </div>

                {/* <div>
                    <TextField className='add' id="standard-basic" label="Camera ID" type='text' value={this.state.id} onChange={this.handleIdChange} />
                </div> */}
                <div>    
                    <TextField className='add' id="standard-basic" label="Server IP" type='text' value={this.state.ip} onChange={this.handleIPChange} />
                </div>
                <Button  type= 'submit' variant="contained" color="primary">
                    Add Server
                </Button>
            </form>    
            {/* <SubForm></SubForm> */}

            <Tables/>
            <Chart/>
        </React.Fragment>
        )
    }


}


// function AdditionalDetails(props) {
//     return(
//         <SubForm IPAddress ={props.ip}></SubForm>
        
//     );
// }


export default Add
