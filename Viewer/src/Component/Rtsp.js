import React, { Component } from 'react'



export class Rtsp extends Component {

    constructor(props) {
        super(props)
    
        this.state = {
             ip: props.ip
        }
             
    }

    componentDidMount() {
        // e.preventDefault();
        // console.log(this.state.comments)
        const requestOptions = {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ 
                                ip: this.state.ip,
                            })
        };
        fetch('http://localhost:9000/stream', requestOptions)
            .then(response => response.json())
            .then(data => console.log(data));
    }
    
    

    render() {
        return (
            <div>
            aisdif
            </div>
        )
    }







}
export default Rtsp
