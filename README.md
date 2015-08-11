# OneM2MC
OneM2M in C++ is an OneM2M (http://www.onem2m.org/) prototype implementation. It targets for a trans-platform, trans-architecture, trans-language universal stack, that can be used in any OneM2M end points, AE, CSE, etc. 

Class relation:

		----------            ---------------
	   |  NSEBase |          |  ResourceBase |
		----+-----            -------^-------
  		    |    	   	   			 -
			|		   	   			 |
			|		        +--------+--------+
			|		   	   	|                 |
			|		   -----+-----       -----+-----
			|		   | CSEBase |       |  AEBase |
			|		   -----------       -----------
            |                                                .....  
		   < >                                               . T . 
	 -----------------                         -------------.....         ------------ 
    |  RequestHandler |                       |  ResourceStore  |<|......|  FakeStore |
     -------^---------                         -------^---------          ------------ 
			-										  -
			|										  |
	 -------+--------- 						  --------+--------- 		 
	|   CSEHandler    }<>--------------------+ CSEResourceStore |	
     -----------------                        ------------------ 




The core protocol engine used is Google ProtoBuf 3, as it can be extended to Java, Python, C#, Object C, etc.
Need boost, protobuf setup and installed.
Development on cygwin, verify on cygwin, Ubuntu.

make all  - build and run gtest, gmock 














	


