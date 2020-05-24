import React from 'react';
import logo from './logo.svg';
import './App.css';
import {Timing} from './Component/timing'
import Home from './Component/Home';
import Add from './Component/Camera/Add'

import { BrowserRouter, Switch, Route, Link } from 'react-router-dom'; 
// import Link from 'react-router-dom'

function App() {
  return (

    <BrowserRouter>
      <Switch>
        <Route path="/"  exact component={Home} />
        <Route path="/add" exact component={Add} />
      </Switch>
    </BrowserRouter>
    
  );
}

export default App;
