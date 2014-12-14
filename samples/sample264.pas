{len : delka podretezce po konkatenaci/ OK}
var	
 	a : string;
 	b : string;
	i : integer;
	
begin
	a := 'abc';
	b := 'abc';
	a := a +b;
	i := length(a);
	write(i)

end.