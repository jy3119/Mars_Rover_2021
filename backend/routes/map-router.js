const express = require('express')

const router = express.Router()

const PostModel = require('../models/map-model');
var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27017/"; //change later

//Import the mongoose module
var mongoose = require('mongoose');

/**************GET REQUESTS **************/
/*
 *	Brief: Get map data for rover 
 *	Path: /api/data/:map
 */

router.get('/linechart', function(req, res) {

    blogPostData(function(result){
      var month_data = result.month_data;
      var number_of_posts_data = result.number_of_posts_data;
  
      console.log(month_data, number_of_posts_data);
      res.render('dashboard/linechart', { 
        title: 'Rover Map',
        datai: JSON.stringify(number_of_posts_data),
        labeli: JSON.stringify(month_data)
       });
    })
  });


/*
 *	Brief: Get state of charge for rover 
 *	Path: /api/plant/control/:plantId
 */







  /**************PUT REQUESTS **************/
/*
 *	Brief: Create a new blank destination entry 
 *	Path: /api/plant/create
 */
 router.put('/create', async (req: Request, res: Response) => {
	const newPlant: IPlant = new Plant()

	try {
		/* Save in db - return number of plants and id */
		await newPlant.save()

		const numPlants: number = await Plant.countDocuments()

		res.json({ numPlants, msg: `Plant ${newPlant.id} has been created.` })
	} catch (error) {
		res.status(500).json({ errors: [{ msg: 'Server error.' }] })
	}
})



  module.exports = router;