{find test : hledani vetsiho retezce v mensim retezci => return value = 0}
var	
 	a : string;
 	b : string;
 	index : integer;
	
begin
	a := 'asdfg';
	b := 'asdfgf';
	index := find(a, b);
	write(index)

end.