# Avoiding congestion in recommender systems

These codes implement the recommendation algorithm proposed in 

Ren, Xiaolong, Linyuan Lü, Runran Liu, and Jianlin Zhang. "Avoiding congestion in recommender systems." New Journal of Physics 16, no. 6 (2014): 063057.; DOI: [10.1088/1367-2630/16/6/063057](https://doi.org/10.1088/1367-2630/16/6/063057)

	
	@article{Ren_2014,
	doi = {10.1088/1367-2630/16/6/063057},
	url = {https://doi.org/10.1088%2F1367-2630%2F16%2F6%2F063057},
	year = 2014,
	month = {jun},
	publisher = {{IOP} Publishing},
	volume = {16},
	number = {6},
	pages = {063057},
	author = {Xiaolong Ren and Linyuan Lü and Runran Liu and Jianlin Zhang},
	title = {Avoiding congestion in recommender systems},
	journal = {New Journal of Physics},
	abstract = {Recommender systems use the historical activities and personal profiles of users to uncover their preferences and recommend objects. Most of the previous methods are based on objects’ (and/or users’) similarity rather than on their difference. Such approaches are subject to a high risk of increasingly exposing users to a narrowing band of popular objects. As a result, a few objects may be recommended to an enormous number of users, resulting in the problem of recommendation congestion, which is to be avoided, especially when the recommended objects are limited resources. In order to quantitatively measure a recommendation algorithmʼs ability to avoid congestion, we proposed a new metric inspired by the Gini index, which is used to measure the inequality of the individual wealth distribution in an economy. Besides this, a new recommendation method called directed weighted conduction (DWC) was developed by considering the heat conduction process on a user–object bipartite network with different thermal conductivities. Experimental results obtained for three benchmark data sets showed that the DWC algorithm can effectively avoid system congestion, and greatly improve the novelty and diversity, while retaining relatively high accuracy, in comparison with the state-of-the-art methods.}
	}


All the results in the paper were produced with the [Visual Studio 2010](https://visualstudio.microsoft.com).

### Codes

The codes mainly include: 
- train_test_split.cpp: Partition the original data set into ten train(90%)/test(10%) sets 

- DWC.cpp: Our proposed DWC algorithm
- ProbS.cpp: The classical probabilistic spreading (ProbS) algorithm proposed by [Zhou, Tao, Jie Ren, Matúš Medo, and Yi-Cheng Zhang. "Bipartite network projection and personal recommendation." Physical Review E 76, no. 4 (2007): 046115.](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.76.046115)
- HeatS.cpp: heat conduction (HeatS) algorithm proposed by [Zhou, Tao, Zoltán Kuscsik, Jian-Guo Liu, Matúš Medo, Joseph Rushton Wakeling, and Yi-Cheng Zhang. "Solving the apparent diversity-accuracy dilemma of recommender systems." Proceedings of the National Academy of Sciences 107, no. 10 (2010): 4511-4515.](https://www.pnas.org/content/107/10/4511)
- Hybrid.cpp: The Hybrid method of the ProbS and Heats, proposed by [Zhou, Tao, Zoltán Kuscsik, Jian-Guo Liu, Matúš Medo, Joseph Rushton Wakeling, and Yi-Cheng Zhang. "Solving the apparent diversity-accuracy dilemma of recommender systems." Proceedings of the National Academy of Sciences 107, no. 10 (2010): 4511-4515.](https://www.pnas.org/content/107/10/4511)

- Gini.cpp: Calculate Gini Coeffcient

