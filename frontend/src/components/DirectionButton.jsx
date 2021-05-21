import React, { Component } from 'react';

class DirectionButton extends Component {
    /*state = {
        count: 0,
        imageUrl: 'https://picsum.photos/200'
      }*/

    render() { 
        return ( 
            <div>
                <button style = {{fontSize:10}} className = 'btn btn-secondary btn-sm'>Forward</button>
            </div>
         );
    }

    /*
    formatCount() {
        const {count} = this.state;
        return count == 0 ? 'Zero' : count; 
    }
    */
}
 
export default DirectionButton;