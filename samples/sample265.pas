{find test : hledani retezce vyuziti expr v parametrech}
var	
 	a : string;
 	b : string;
 	index : integer;
	
begin
	a := 'asdfg';
	b := 'asd';
	index := find(a, copy(b, (0+1), length(b))); {1}
	write(index)

end.