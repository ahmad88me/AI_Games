/* 
    Andrea Centeno
    10-10138
    Sergio Teran
    11-11020
    sep-dic 2017
*/
class PrioritySteering{
	public:
	//  Holds a list of BlendedSteering instances, which in turn
	//  contain sets of behaviors with their blending weights.
	deque<BlendedSteering*> &groups;

	//  Holds the epsilon parameter, should be a small value
	GLfloat epsilon = 10^(-27);

	// Returns the acceleration required.
	SteeringOutput getSteering(){
		SteeringOutput steering;
		// Go through each group
		for( deque<BlendedSteering*>::iterator groupI = groups.begin(); groupI != groups.end(); ++groupI ){
			// 	Create the steering structure for accumulation
			steering = (*groupI)->getSteering();

			// Check if we’re above the threshold, if so return
			if(steering.linear.length() > epsilon || abs(steering.angular) > epsilon){
				return steering;
			};
		};

		// If we get here, it means that no group had a large
		// enough acceleration, so return the small
		// acceleration from the final group.
		return steering;
	};
};