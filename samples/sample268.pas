{len : delka podretezce po konkatenaci s expr/ OK}
var	
 	a : string;
 	b : string;
	i : integer;
	
begin
	a := 'abc';
	i := length(a+a+a); {9}
	write(i)

end.