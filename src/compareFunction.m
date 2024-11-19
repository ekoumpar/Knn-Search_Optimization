load('large.mat');
k=10;
[Kindex, K] = knnsearch(CORPUS,QUERY, 'K', k);

data = load('output.mat');

K=sort(K,2);
data.K=sort(data.K,2);

K=round(K,4);
data.K=round(data.K,4);

%Kindex=sort(Kindex,2);
%data.Kindex=sort(data.Kindex,2);

% equal elements
equal_elements = K == data.K;
percentage_equal = sum(equal_elements(:)) / numel(K) * 100;
    
disp(['Percentage of equal elements: ', num2str(percentage_equal), '%']);

% equal indexes
%equal_elements = Kindex == data.Kindex;
%percentage_equal = sum(equal_elements(:)) / numel(Kindex) * 100;
    
%disp(['Percentage of equal indexes:  ', num2str(percentage_equal), '%']);
