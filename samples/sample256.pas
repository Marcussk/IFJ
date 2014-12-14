{find test : hledani retezce v  retezci ERROR }
var	
 	a : string;
 	b : string;
 	index : integer;
	
begin
	a := 'asdfg';
	b := 'fg';
	index := find(a, b);
	write(index)

end.