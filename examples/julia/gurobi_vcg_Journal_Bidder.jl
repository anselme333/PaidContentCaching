##########################################################################################################################################
# Libralies
using JuMP
using Gurobi
using PyPlot
using DataStructures
using StatsBase # Package that provides basic support for statistics
##########################################################################################################################################
# Configurations for reverse auction
t=[] # ISP profit array Initiliazation
z=[] # Number of customer array Initiliazation
u=[] # ICSP profit array Initiliazation
v=[] # CP profit array Initiliazation
w=[]# No cache
q=[] # With cache
s=[] # Cache size

distributor=10
while distributor<=100
distributor_price = rand(5:10,distributor) # Vector of bids in the bundle
content_size = rand(5:500,distributor) # Vector of content size received in the bundle in terms of megabyte
Number_distributor = size(distributor_price)
x = zeros(size(distributor_price)) # Initiliazation of decision vector to 0
y = zeros(size(distributor_price)) # Initiliazation of decision vector to 0
cache_size =100000 # Cache size per each router 100000 MB
cache_price = 0.003625 # Memory cost per Mb in USD
link_capacity = sum(content_size)
access_bandwidth = 50  # 50 usd for monthly access bandwidth per 1GB
transit_fee = 0.63 # transit fee 0.63 usd per Mbps
bids = distributor_price
#bids = distributor_price .* content_size
######################################################################################################################################################
# Determine the baseline content size needed by ISP
global F=content_size
count= countmap(F) # Make dictionary of content size and number of accurence
max_count=maximum(values(count)) # Maximm count for the content size
baseline_size=reduce((p, q) -> count[p] >= count[q] ? p : q, keys(count)) #baseline content size needed by ISP
#println("Baseline Content Size: ", baseline_size )
#println("Number of occurence in bids ", max_count )
######################################################################################################################################################
# Determine the payment  that minimize the total payment with GurobiSolver and without winner with winner
R=bids
i = 1;
while i <= length(R) && content_size[i] .>=baseline_size
	i += 1
	min_bid = minimum(R)
global k = findfirst(bids, min_bid)
	x[k]=1 # update decision vector based on the winner
	println("Decison variable: ", x)
	println("Winner: ", k)
 end
# With winner
number_router=200 # Number of the router inside the ISP
total_cache_size = cache_size * number_router #In  terms of megabyte
vcg= Model(solver=GurobiSolver(Presolve=0))
@variable(vcg, x,Bin)
@objective(vcg, Min, sum(R) * x)
@constraint(vcg, sum(content_size) * x <= link_capacity)
@constraint(vcg, sum(x)<=1)
@constraint(vcg, sum(content_size) * x>=baseline_size)
@constraint(vcg, sum(content_size) * x <= total_cache_size)
println("The optimization problem to be solved (winner encluded) is:")
print(vcg)
status = solve(vcg)
println("Optimal objective: ",getobjectivevalue(vcg),
    ". x = ", getvalue(x))

######################################################################################################################################################
# Determine the payment  that minimize the total payment with GurobiSolver and without winner
		bid_without_winner= collect(drop(R,k)) # Remove winner
		content_size_without_winner=collect(drop(content_size,k))
		j = 1;
		while j <= length(bid_without_winner) && content_size[i] .>=baseline_size
			j += 1
		min_bid_no_winner = minimum(bid_without_winner)
		global l = findfirst(bid_without_winner, min_bid_no_winner)
			y[l]=1 # update decision vector based on the winner
		 end
# With winner
vcg1 =  Model(solver=GurobiSolver(Presolve=0))
@variable(vcg1, y,Bin)
@objective(vcg1, Min, sum(bid_without_winner) * y)
@constraint(vcg1, sum(content_size) * y <= link_capacity)
@constraint(vcg1, sum(y)<=1)
@constraint(vcg1, sum(content_size) * y>= sum(baseline_size))
@constraint(vcg1, sum(content_size) * y <= total_cache_size)
println("The optimization problem to be solved (winner excluded) is:")
print(vcg1)
status = solve(vcg1)
println("Optimization: ", status)
println("Objective value: ", getObjectiveValue(vcg1))
println("y = ", getValue(y)) # getValue(decision_variable) will give the optimum value of the associated decision variable

######################################################################################################################################################
optimum_price=abs(getObjectiveValue(vcg1)-getObjectiveValue(vcg))
println("Optimum Price: ", optimum_price)
##########################################################################################################################################
######################################################################################################################################################
		 # Determine the payment  that minimize the total payment with standard VCG

tatal_evaluation_withoutwinner0=sum(bid_without_winner) # Total evaluation without winner
tatal_evaluation_withwinner0=sum(R) # Total evaluation with winner
global social_optimum_price0= abs(tatal_evaluation_withoutwinner0-tatal_evaluation_withwinner0)
social_optimum_price_position0= findfirst(R, social_optimum_price0)
global optimum_price0 = R[social_optimum_price_position0 + 1] # Second price

##########################################################################################################################################


global customer_content_price = optimum_price
global customer_content_price0 = optimum_price0
# Determine the profit of content povider and Internet Service providers with variable number of customer [10000,100000]
# provider_profits
number_router=200 # Number of the router inside the ISP
total_cache_size = cache_size * number_router #In  terms of megabyte
# Determine the profit of content providers
numbercostomer=10000 #Number of customer Interested in paid content
cache_cost =cache_price * ( total_cache_size) # the cost  of cache implementaion
monthly_volume_demand = ((baseline_size ) * 30) * numbercostomer # We assume that each customer request one file per day
monthly_volume_Without_Cache = monthly_volume_demand  # monthly traffic without cache
monthly_volume_WithCache0 = monthly_volume_Without_Cache - total_cache_size
global monthly_volume_WithCache=max(0, monthly_volume_WithCache0)

cache_hit= total_cache_size/monthly_volume_demand # cache hit ratio equals to total hits devided by the sum of hit and miss
cache_miss=1-cache_hit
# Total ISP revenue P1 for gurobi, p10 for standard VCG
p1 = (numbercostomer * access_bandwidth)  + (numbercostomer * (optimum_price  * 1)) + (0.15*((numbercostomer-1) * (customer_content_price * ((total_cache_size/baseline_size)-1)))) # When ISP buy, cache and sell the contnet
p10 = (numbercostomer * access_bandwidth)  + (numbercostomer * (optimum_price0  * 1)) + (0.15*((numbercostomer-1) * (customer_content_price0 * ((total_cache_size/baseline_size)-1)))) # When ISP buy, cache and sell the contnet

# Total ICSP revenue P1 for gurobi, p10 for standard VCG
p2 =  ((monthly_volume_demand  * transit_fee ) * 0.95) + (numbercostomer * (optimum_price  * 1))+  (0.15*((numbercostomer-1) * (optimum_price  * ((monthly_volume_WithCache/baseline_size)-1))))
p20 =  ((monthly_volume_demand  * transit_fee ) * 0.95) + (numbercostomer * (optimum_price0  * 1))+  (0.15*((numbercostomer-1) * (optimum_price0  * ((monthly_volume_WithCache/baseline_size)-1))))

# Total ISP revenue P1 for gurobi, p10 for standard VCG
#p1 = (numbercostomer * ((access_bandwidth/1000)*monthly_volume_demand)) + ((numbercostomer * (optimum_price  * 1))+ (0.15*((numbercostomer-1) * (customer_content_price * cache_hit)))) # When ISP buy, cache and sell the content
#p10 = (numbercostomer * ((access_bandwidth/1000)*monthly_volume_demand)) + ((numbercostomer * (optimum_price0  * 1))+ (0.15*((numbercostomer-1) * (customer_content_price0 * cache_hit)))) # When ISP buy, cache and sell the content

# Total ICSP revenue P1 for gurobi, p10 for standard VCG
#p2 =  ((monthly_volume_demand * transit_fee ) * 0.95) + ((numbercostomer * (optimum_price  * 1))+(0.15*((numbercostomer-1) * (optimum_price  * cache_miss))))
#p20 =  ((monthly_volume_demand * transit_fee ) * 0.95) + ((numbercostomer * (optimum_price0  * 1))+(0.15*((numbercostomer-1) * (optimum_price0  * cache_miss))))


# Monthly transit fee
p2_normal = (monthly_volume_demand  * transit_fee ) * 0.95
isp_profit = p1-p2_normal -cache_cost
isp_profit0 = p10-p2_normal-cache_cost
cp=(numbercostomer * (optimum_price  * 1)) + (0.70 * ((numbercostomer-1) * (optimum_price *((monthly_volume_demand/baseline_size)-1))))# only cp
#ICSP profits due to in network caching
cache_cost_ICSP= monthly_volume_WithCache * cache_price
ICSP_profit =p2-cache_cost_ICSP
ICSP_profit0 =p20-cache_cost_ICSP

z=append!(z, Number_distributor)
t=append!(t, isp_profit)
u=append!(u, cp)
v=append!(v, ICSP_profit)
w=append!(w, monthly_volume_Without_Cache)
q=append!(q, monthly_volume_WithCache)
s=append!(s, total_cache_size)
distributor=distributor+10
end
##########################################################################################################################################
# In-Network Caching Profits graph
fig = figure("pyplot_multiaxis",figsize=(10,10))
p =plot(z,t,color="red",marker="o",markersize=10, linewidth=2.0,label=" ISP Profit")
ax = gca()
title("In-Network Caching Profits", fontsize=20)
grid("on")
xlabel("Number of bidders", fontsize=20)
font1 = Dict("color"=>"red")
ylabel("ISP Profit",fontdict=font1, fontsize=20)
setp(ax[:get_yticklabels](),color="red", fontsize=20) # Y Axis font formatting
legend(loc="upper left",fancybox="true", fontsize=20)
################
#  Other Axes  #
################
new_position = [0.06;0.06;0.77;0.91] # Position Method 2
ax[:set_position](new_position) # Position Method 2: Change the size and position of the axis
#fig[:subplots_adjust](right=0.85) # Position Method 1
ax2 = ax[:twinx]() # Create another axis on top of the current axis
font2 = Dict("color"=>"purple")
ylabel("CP Profit",fontdict=font2, fontsize=20)
p =plot(z,u,color="purple",marker="*", markersize=20, linewidth=2.0, label="CP Profit")# Plot a basic line
ax2[:set_position](new_position) # Position Method 2
setp(ax2[:get_yticklabels](),color="purple", fontsize=20) # Y Axis font formatting
legend(loc="upper center",fancybox="true", fontsize=20)

ax3 = ax[:twinx]() # Create another axis on top of the current axis
ax3[:spines]["right"][:set_position](("axes",1.12)) # Offset the y-axis label from the axis itself so it doesn't overlap the second axis
font3 = Dict("color"=>"green")
ylabel("ICSP Profit",fontdict=font3, fontsize=20)
p = plot_date(z,v, color="green",linestyle="-",linewidth=2.0, marker="x", markersize=20,label="ICSP Profit") # Plot a basic line
ax3[:set_position](new_position) # Position Method 2
setp(ax3[:get_yticklabels](),color="green", fontsize=20) # Y Axis font formatting
legend(loc="upper right",fancybox="true", fontsize=20)
axis("tight")
# Enable just the right part of the frame
ax3[:set_frame_on](true) # Make the entire frame visible
ax3[:patch][:set_visible](false) # Make the patch (background) invisible so it doesn't cover up the other axes' plots
ax3[:spines]["top"][:set_visible](false) # Hide the top edge of the axis
ax3[:spines]["bottom"][:set_visible](false) # Hide the bottom edge of the axis
fig[:canvas][:draw]() # Update the figure
gcf() # Needed for IJulia to plot inline
