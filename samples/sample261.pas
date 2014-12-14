{copy test : kopirovani vetsiho retezce do mensiho/ OK}
var	
 	a : string;
	b : string;
	
begin
	a := 'abc';
	b := 'abcabcabcfff';
	a := copy(b, 1,12);
	write(a)

end.