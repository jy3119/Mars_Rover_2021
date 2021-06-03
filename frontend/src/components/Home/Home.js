import React from 'react';

/* Brief 
    Fetch coordinates 
*/

const Home = () => {
    return (
        <div>
            Map
            Warnings
        </div>
    )
}

export default Home; 


    getBlogPost = () => {
        axios.get('/api')
            .then((response)=> {
                const data = response.data;
                console.log('Datta received');
            })
            .catch
    }

