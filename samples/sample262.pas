{copy test : kopirovani mensiho retezce do vetsiho/ OK}
var	
 	a : string;
	b : string;
	
begin
	a := 'abc';
	b := 'abcabcabcfff';
	b := copy(a, 1,3);
	write(b)

end.