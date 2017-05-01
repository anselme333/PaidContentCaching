#include <iostream>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <chrono>
#include <algorithm>
#include <string>
#include <ostream>
#include <fstream>
#include <set>
#include <functional>
using namespace std;


namespace vcg_auction {
/*
This class is the storage container for the items information.
*/
class AuctionInfo {
public:
	/*
	v = total value of items already selected
	v(i+1) = the value of the next item that can be selected
	*/
	int numberprovider;
	int position_winner_price;
	vector<double> bids ;	// Vector of content in the bundle
	vector<double> content_size_demand; //  Vector of content size
	vector<double> provider_name;
	vector<double> content_size_received;
	vector<double> result_vector;
	vector<double> price_vector1;
	vector<double> price_vector2;
	vector<double> bid_without_winner;
	vector<double> content_without_winner;
    char winner;
    double content_todeliver;
	bool sub_auction;
	bool sel_auction;
	double total_evaluation;
	double optimum_price;
    double result1;
    double result2;
    double tatal_evaluation;
    double content_provider_utility;
    double content_provider_profit;


	// Constructor
    AuctionInfo()
	{
    	position_winner_price=0;
    	numberprovider = 0;
    	result1 = 0;
    	result2 = 0;
		sub_auction= true;
		sel_auction = false;
		total_evaluation = 0;
		optimum_price = 0;
		winner = 0;
	    content_todeliver=0;
	    int position_winner_price = 0;
	    tatal_evaluation = 0;
	    content_provider_utility = 0;
	    content_provider_profit = 0;
	}
int winner_determination( sub_auction, provider_name, numberprovider,sub_auction, bids, content_size_demand,content_size_received)
  {
if(sub_auction == true)
{
	result_vector= sort(bids.begin(), bids.end());

	if(!result_vector.empty())
	   {
	       if(content_size_received >=content_size_demand)
	       {
	    	   for (long i = 0; i < bids.size(); ++i)
	    	   {
	    	    price_vector1 = bids[i]* content_size_received[i];
	    	    tatal_evaluation = accumulate( price_vector1.begin(),  price_vector1.end(), 0.0); //total evaluation
	    	    result1 = min_element(price_vector1.begin(), price_vector1.end());// Return minimum bid
	    	    int position_winner_price = find(price_vector1.begin(),price_vector1.end(),"result");// check the position of the minimum bid in price vectors
	    	    winner = provider_name[position_winner_price]; // check in content provider, the bidder who provided minimum bids
	    	    content_todeliver = content_size_received[position_winner_price];// check the size of the content that will be supplied

	    	   }
	       }
	   }
}
sel_auction = true;
std::cout << "The winner of auction" << optimum_price << std::endl;
return winner;

  }
int winner_determination( provider_name, numberprovider,sub_auction, bids, content_size_demand, sel_auction)
 {

	bid_without_winner = bids.erase(bids.begin()+ position_winner_price);
	content_without_winner = content_size_received.erase(content_size_received.begin()+ position_winner_price);

	if(sel_auction == true)
	{

		for (long i = 0; i < bids.size(); ++i)
			    	   {
			    	   if(price_vector1[i]>=0)
			    	   {
			    		   price_vector2 = bid_without_winner[i] * content_without_winner[i];
			    		   tatal_evaluation = accumulate( price_vector1.begin(),  price_vector1.end(), 0.0); //total evaluation without winner
			    		   result2 = min_element(price_vector1.begin(), price_vector1.end());// Return minimum bid without winner
			    		   optimum_price=result2-price_vector1;   //Optimum price for buying content
			    		   content_provider_utility= optimum_price - bids[position_winner_price];//Utility of content provider
			    	   }

			    	   }
	}
	std::cout << "Optimum price for buying content" << optimum_price << std::endl;
	std::cout << "Utility of content provider " << content_provider_utility << std::endl;
	return optimum_price;
 }
void optimzation_feasible(optimum_price)
{

}

	friend ostream& operator<<(ostream &out, list<AuctionInfo>::iterator &item);
};
}
