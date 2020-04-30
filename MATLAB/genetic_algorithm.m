clear all %========================================================================== % inputting the variables %==========================================================================
generations = 1000; % number of generations
crossover_probability = 40; % mating probability
mutation_probability = 10; % mutation probability
genes = 100; % number of genes in a chromosome, also max fitness size chromosomes = 100; % number of chromosomes / population size wheel_spins = chromosomes; %number of roulette wheel spins
%========================================================================== % generate initial population %==========================================================================
population = randi([0, 1], chromosomes, genes); %population for z = 1:generations
%========================================================================== % assess fitness
%==========================================================================
for i = 1:chromosomes
fitness(i) = sum(population(i,:));
end
%========================================================================== % roulette selection
%==========================================================================
fitness_total = sum(fitness); %finding the total fitness of the population
probability = (fitness/fitness_total)*100; % x by 100 for easier random number generation
cumulative_prob = cumsum(probability);
for w = 1:wheel_spins
a = randi([1, (genes-1)]); %spinning the wheel, by generating a
random number x.
k = find(cumulative_prob>a,1);
parents(w,:)= population(k,:); %generating a parent matrix of the selected genes
end
%========================================================================== % Crossover
%==========================================================================
    %selecting two parents
for b = 1:(wheel_spins-1)
parent_1 = parents(b,:); c=b+1;
parent_2 = parents(c,:);
f = randi([0, 100]); %deciding whether to crossover based off crossover probability
if f < crossover_probability
e = randi([0, genes]); %determining the point where you draw
the crossover line
cross_1 = parent_1(1:e);
keep_1 = parent_1((e+1):genes);
cross_2 = parent_2(1:e);
keep_2 = parent_2((e+1):genes);
child_1 = [cross_2 keep_1]; %children are a combination of the
child_2 = [cross_1 keep_2];
else
end
children(b,:) = [child_1]; %this is now a matrix of the offspring children(c,:) = [child_2];
end
%========================================================================== % Mutation
%==========================================================================
for i = 1:wheel_spins
g = randi([0, 100]); %deciding whether to mutate based off that
probability
if g < mutation_probability
random_chrom = randi([1, chromosomes]); random_gene = randi([1, genes]);
if children(random_chrom, random_gene) ==0
children(random_chrom, random_gene) = 1;
else
end end
end
%========================================================================== % New population
%==========================================================================
population = children; fitness_avg(z) = mean(fitness);
end