import React from 'react';
import logo from './logo.svg';
import './App.css';
import {Timing} from './Component/timing'
import Home from './Component/Home';
import Add from './Component/Camera/Add'
import AddServer from './Component/Server/AddServer'
import 'bootstrap/dist/css/bootstrap.min.css';

import { BrowserRouter, Switch, Route, Link } from 'react-router-dom'; 
import Rtsp from './Component/Rtsp';

function App() {
  return (
    <BrowserRouter>
      <Switch>
        <Route path="/"  exact component={Home} />
        <Route path="/camera" exact component={Add} />
        <Route path="/Server" exact component={AddServer} />
        <Route path="/view" exact component={Rtsp} />
      </Switch>
    </BrowserRouter>
  );
}

export default App;
