

var net = require('net');

var count = 0;

var svr = net.createServer(function(c){
	console.log('\n');
	console.log('client connected at : ' + (++count));

	console.log('------------------------------------')
	c.on('data', function(data){
	    console.log(data.toString());
	    });

	c.on('end', function(){
	    console.log('------------------------------------')
	    console.log('client goodbye ' + count);
	    });

	});

svr.listen(4040, function(){
	console.log('server is listening...');
	});

