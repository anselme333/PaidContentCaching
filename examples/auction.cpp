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
#include <stdlib.h>
//#include <glpk.h>

using namespace std;
namespace vcg_auction {
class AuctionInfo{
public:
	vector<double> result_vector;
	vector<double> price_vector1;
	vector<double> price_vector2;
	vector<double>::const_iterator bid_without_winner;
	vector<double>::const_iterator content_without_winner;
    char winner;
    double content_todeliver;
	bool sel_auction;
	double total_evaluation;
	double optimum_price;
	double result1;
	double result2;
    double minimum_tatal_evaluation;
    double tatal_evaluation_withoutwinner;
    double content_provider_utility;
    double isp_profit;
    double high_bid;
    double customer_content_price;
    double p1;
    double p2;
    double cache_cost;
    unsigned int position_winner_price;
    unsigned int position__without_winner_price;

	// Constructor
    AuctionInfo()
	{
    	cache_cost=0;
		sel_auction = false;
		total_evaluation = 0;
		optimum_price = 0;
		winner = 0;
		result1=0;
		result2=0;
	    content_todeliver=0;
	    minimum_tatal_evaluation  = 0;
	    content_provider_utility = 0;
	    isp_profit = 0;
	    tatal_evaluation_withoutwinner = 0;
	    high_bid = 0;
	    customer_content_price = 0;
	    p1=0;
	    p2=0;
	    position_winner_price=0;
	    position__without_winner_price=0;

	}
void winner_determination(bool sub_auction, vector<char> provider_name, vector<double> bids, vector<double> content_size_demand, vector<double> content_size_received)
  {
auto t1 = std::chrono::high_resolution_clock::now();
clock_t start = clock();
if(sub_auction)
{
	sort(bids.begin(), bids.end());
	for (unsigned int i = 0; i < bids.size(); ++i)
		    	   {
	      if(content_size_received.size() >= content_size_demand.size())
	       {
	    	  price_vector1.push_back(bids[i] * content_size_received[i]);
	    	  result1=*min_element(price_vector1.begin(), price_vector1.end());// Return minimum bid
	    	  minimum_tatal_evaluation = accumulate( price_vector1.begin(),  price_vector1.end(), result1); //total evaluation
	    	  position_winner_price = std::find(price_vector1.begin(),price_vector1.end(),result1)- price_vector1.begin();// check the position of the minimum bid in price vectors
	       }
	    if( position_winner_price  < price_vector1.size() )
	    	  	 {
	    	  	    	    	 winner = provider_name[position_winner_price]; // check in content provider, the bidder who provided minimum bids
	    	  	    	         content_todeliver = content_size_received[position_winner_price];// check the size of the content that will be supplied
	    	  	  }

	       }
	  }

sel_auction = true;
std::cout << "The price of the winner :\n" <<result1<< std::endl;
std::cout << "content to deliver:\n" <<content_todeliver<< std::endl;
std::cout << "Total evaluation:\n" <<minimum_tatal_evaluation<< std::endl;
  }
void price_determination( vector<double> bids,vector<double> content_size_demand, vector<double> content_size_received)
 {
	price_vector1.erase(std::remove(price_vector1.begin(), price_vector1.end(), result1),  price_vector1.end());
	if(sel_auction)
	{
		for (unsigned int i = 0; i < bids.size(); ++i)
			    	   {
			    	   if(price_vector1[i]>=0)
			    	   {
			    		   price_vector2.push_back(price_vector1[i]);
			    		   result2 = *min_element(price_vector2.begin(), price_vector2.end());// Return minimum bid without winner
			    	   }
			    	   tatal_evaluation_withoutwinner = accumulate( price_vector2.begin(),  price_vector2.end(), 0); //total evaluation without winner
			    	   position__without_winner_price = std::find(price_vector2.begin(),price_vector2.end(),0)- price_vector2.begin();// check the position of the minimum bid in price vectors
			    	   optimum_price = tatal_evaluation_withoutwinner- minimum_tatal_evaluation ;  //Optimum price for buying content
			    	   content_provider_utility= optimum_price -result1;//Utility of content provider

			    	   }
	}
	std::cout << " price without winner:\n" <<result2<< std::endl;
	std::cout << "Optimum price for buying content\n" << optimum_price << std::endl;
	std::cout << "Utility of content provider\n " << content_provider_utility << std::endl;
	std::cout << "tatal_evaluation_withoutwinner \n " << tatal_evaluation_withoutwinner  << std::endl;

 }

void customer_price(vector<double> bids, int numbercostomer)
{
	customer_content_price = optimum_price ;
	std::cout << "customer_content_price  \n " <<customer_content_price << std::endl;
}

void provider_profits(int numbercostomer, int number_router, double cache_price, double cache_size, double payload, double access_bandwidth, double transit_fee )
{
double montly_volume;
montly_volume = ((payload * content_todeliver) * 43200)/1024;
p1 = (numbercostomer * access_bandwidth) + (numbercostomer * (optimum_price * montly_volume) );
//p1 = (numbercostomer * access_bandwidth);
p2 =  ((montly_volume  *  transit_fee ) + (numbercostomer *(optimum_price * montly_volume ))) * 0.95;
//p2 = (montly_volume * transit_fee ) * 0.95;
//cache_cost = ((cache_price) * (montly_volume )) * number_router;
cache_cost = ((cache_price) * (montly_volume ));
isp_profit = p1-p2-cache_cost;
//isp_profit = p1-p2;
std::cout << "cache_cost  \n " <<cache_cost << std::endl;
std::cout << "isp_profit \n " <<isp_profit  << std::endl;
std::cout << "P1 \n " << p1  << std::endl;
std::cout << "p2  \n " <<p2   << std::endl;
std::cout << "montly_volume  \n " << montly_volume    << std::endl;
clock_t end = clock();
//auto t2 = std::chrono::high_resolution_clock::now();
//double timing = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
//double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
//int iter = 1;
//cout << "Num Iterations:\t" << iter << endl;
//cout << "Total time:\t" << timing / 1000.0 << endl;
//cout << "Total CPU time:\t" << time << endl;


}

};//end of namespace vcg_auction
}

int main(int argc, const char *argv[]) {

	 /*   glp_prob *lp;
	    int ia[1+1000], ja[1+1000];
	    double ar[1+1000], z, x1, x2, x3;
	    lp = glp_create_prob();
	    glp_set_prob_name(lp, "sample");
	    glp_set_obj_dir(lp, GLP_MAX);
	    glp_add_rows(lp, 3);
	    glp_set_row_name(lp, 1, "p");
	    glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 100.0);
	    glp_set_row_name(lp, 2, "q");
	    glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 600.0);
	    glp_set_row_name(lp, 3, "r");
	    glp_set_row_bnds(lp, 3, GLP_UP, 0.0, 300.0);
	    glp_add_cols(lp, 3);
	    glp_set_col_name(lp, 1, "x1");
	    glp_set_col_bnds(lp, 1, GLP_LO, 0.0, 0.0);
	    glp_set_obj_coef(lp, 1, 10.0);
	    glp_set_col_name(lp, 2, "x2");
	    glp_set_col_bnds(lp, 2, GLP_LO, 0.0, 0.0);
	    glp_set_obj_coef(lp, 2, 6.0);
	    glp_set_col_name(lp, 3, "x3");
	    glp_set_col_bnds(lp, 3, GLP_LO, 0.0, 0.0);
	    glp_set_obj_coef(lp, 3, 4.0);
	    ia[1] = 1, ja[1] = 1, ar[1] = 1.0; //a[1,1]
	    ia[2] = 1, ja[2] = 2, ar[2] = 1.0; //a[1,2]
	    ia[3] = 1, ja[3] = 3, ar[3] = 1.0; // a[1,3]
	    ia[4] = 2, ja[4] = 1, ar[4] = 10.0; // a[2,1]
	    ia[5] = 3, ja[5] = 1, ar[5] = 2.0; // a[3,1]
	    ia[6] = 2, ja[6] = 2, ar[6] = 4.0; // a[2,2]
	    ia[7] = 3, ja[7] = 2, ar[7] = 2.0;
	    ia[8] = 2, ja[8] = 3, ar[8] = 5.0;
	    ia[9] = 3, ja[9] = 3, ar[9] = 6.0;
	    glp_load_matrix(lp, 9, ia, ja, ar);
	    glp_simplex(lp, NULL);
	    z = glp_get_obj_val(lp);
	    x1 = glp_get_col_prim(lp, 1);
	    x2 = glp_get_col_prim(lp, 2);
	    x3 = glp_get_col_prim(lp, 3);
	    std::cout << z <<x1 << x2 << z<<"." <<  x1<<"." <<  x2<< x3<<"." << std::endl;
	    glp_delete_prob(lp);
*/
	    int numberprovider = 10;// total number of content provider
		int numbercostomer = 5000000; // total number of customer
		vector<double> bids{20,18,30,70,20,17,16,20,18,21,30,18,50,70,20,17,16,20,18,21,30,18,50,70,20,17,16,20,18,21,30,18,50,70,20,17,16,20,18,21};	// Vector of bids in the bundle
		//vector<double> content_size_demand{30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30}; //  Vector of content size demand in the bundle
		//vector<double> content_size_received{30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};//Vector of content size received in the bundle
		//vector<double> content_size_demand{20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20}; //  Vector of content size demand in the bundle
		//vector<double>  content_size_received{20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};//Vector of content size received in the bundle
		//vector<double> content_size_demand{40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40}; //  Vector of content size demand in the bundle
		//vector<double> content_size_received{40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40};//Vector of content size received in the bundle
		//vector<double> content_size_demand{50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50}; //  Vector of content size demand in the bundle
		//vector<double> content_size_received{50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50};//Vector of content size received in the bundle
		vector<double> content_size_demand{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10, 10,10,10,10,10,10,10,10,10,10}; //  Vector of content size demand in the bundle
		vector<double> content_size_received{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10, 10,10,10,10,10,10,10,10,10,10};//Vector of content size received in the bundle
		vector<char> provider_name{'a','b','c','d','e','f','g','h','i','j','k','l','m','n','0','p','r','s','t','u','aa','ba','cc','da','ea','fa','gg','ph','ki','uj','aa','ba','cc','da','ea','fa','gg','ph','ki','uj'};// Content Provider
		double cache_size=1000;// Cache size
		double cache_price= 0.003625; // Memory cost per Mbin USD
		int number_router=100;
		double payload=1024;
		double access_bandwidth= 50; // 50 usd for monthly access bandwidth
	    double transit_fee = 0.63;//  transit fee per Mbps
		bool sub_auction= true;
		vcg_auction::AuctionInfo vcg;
		vcg.winner_determination(sub_auction, provider_name, bids, content_size_demand, content_size_received);
		vcg.price_determination(bids, content_size_demand, content_size_received);
		vcg.customer_price(bids,numbercostomer);
		vcg.provider_profits(numbercostomer, number_router, cache_price, cache_size, payload, access_bandwidth, transit_fee);
		//Vector of consumers
		  vector<char> consumers{'leaf-327','leaf-1837','leaf-631','leaf-694','leaf-1894','leaf-1795','leaf-1814','leaf-3997','leaf-1341','leaf-1770','leaf-1786','leaf-1788','leaf-1789','leaf-1794','leaf-1805','leaf-1806','leaf-1813','leaf-1819','leaf-3988','leaf-3991','leaf-4246','leaf-4248','leaf-4254','leaf-4360','leaf-4367','leaf-3911','leaf-4356','leaf-1817','leaf-1829','leaf-1822','leaf-1827','leaf-1834','leaf-1831','leaf-1870','leaf-1833','leaf-1872','leaf-3912','leaf-3919','leaf-4256','leaf-4345','leaf-1835','leaf-1838','leaf-1839','leaf-1840','leaf-1841','leaf-1842','leaf-1843','leaf-1844','leaf-1845','leaf-1846','leaf-1847','leaf-1848','leaf-1850','leaf-1851','leaf-1852','leaf-1853','leaf-1854','leaf-1855','leaf-1856','leaf-1857','leaf-1858','leaf-1859','leaf-1862','leaf-1867','leaf-4344','leaf-1876','leaf-1868','leaf-1877','leaf-1889','leaf-1881','leaf-1898','leaf-1899','leaf-1924','leaf-1900','leaf-1901','leaf-1905','leaf-1912','leaf-1907','leaf-1915','leaf-1916','leaf-1917','leaf-1918','leaf-1919','leaf-1920','leaf-1926','leaf-4302','leaf-3915','leaf-3914','leaf-3923','leaf-3924','leaf-3925','leaf-3983','leaf-4259','leaf-3986','leaf-4361','leaf-3968','leaf-3959','leaf-3960','leaf-3964','leaf-3966','leaf-3967','leaf-3969','leaf-3970','leaf-3971','leaf-3972','leaf-3947','leaf-3993','leaf-4326','leaf-3994','leaf-3995','leaf-4215','leaf-4219','leaf-4235','leaf-4225','leaf-4230','leaf-4231','leaf-4232','leaf-4233','leaf-4234','leaf-4236','leaf-4237','leaf-4244','leaf-4245','leaf-4287','leaf-4240','leaf-4310','leaf-4315','leaf-4270','leaf-4271','leaf-4273','leaf-4275','leaf-4276','leaf-4279','leaf-4280','leaf-4281','leaf-4283','leaf-4284','leaf-4285','leaf-4286','leaf-4288','leaf-4291','leaf-4298','leaf-4299','leaf-4301','leaf-4303','leaf-4278','leaf-4297','leaf-4292','leaf-4339','leaf-4327','leaf-4311','leaf-4312','leaf-4308','leaf-4317','leaf-4307','leaf-4313','leaf-4318','leaf-4319','leaf-4320','leaf-4322','leaf-4328','leaf-4330','leaf-4331','leaf-4333','leaf-4334','leaf-4337','leaf-4338','leaf-4348','leaf-4349'};
		  std::cout << "consumers\n " <<consumers.size()<< std::endl;
		  //Vector of core routers
		  vector<char> corerouters{'bb-781','bb-3921','bb-798','bb-3931','bb-3932','bb-3933','bb-3934','bb-3941','bb-3965','bb-3974','bb-3976','bb-3978','bb-3979','bb-3980','bb-3981','bb-1784','bb-1793','bb-1796','bb-1797','bb-1798','bb-1799','bb-1800','bb-1801','bb-1802','bb-1803','bb-1804','bb-1807','bb-1808','bb-1809','bb-1810','bb-1811','bb-1812','bb-1836','bb-3946','bb-3956','bb-3942','bb-3945','bb-3958','bb-3937','bb-3938','bb-3939','bb-3940','bb-3948','bb-3949','bb-3951','bb-3952','bb-3953','bb-3955','bb-3957','bb-3961','bb-3962','bb-3963','bb-3975','bb-3935','bb-3954','bb-4220','bb-4226','bb-4227','bb-4228','bb-4229','bb-4238','bb-4239','bb-4242','bb-4243','bb-4336'};
		  std::cout << "corerouters \n " <<corerouters.size() << std::endl;
		  //Vector of gateways
		  vector<char> gateways{'gw-4223','gw-1875','gw-1790','gw-4222','gw-1791','gw-1792','gw-3928','gw-3973','gw-3929','gw-3930','gw-3977','gw-3943','gw-4306','gw-1787','gw-4216','gw-1785','gw-4217','gw-1897','gw-3927','gw-1832','gw-1863','gw-1830','gw-3917','gw-4350','gw-1849','gw-1861','gw-1865','gw-1866','gw-4221','gw-1864','gw-4304','gw-4305','gw-1913','gw-3922','gw-3926','gw-3992','gw-4266','gw-4218','gw-4224','gw-4274','gw-4268','gw-4324','gw-4325','gw-4329','gw-4332'};
		  std::cout << "gateways \n " << gateways.size()<< std::endl;
	return 0;
}
