{copy test : kopirovani prazdneho retezce}
var	
 	a : string;
	b : string;
	
begin
	a := 'abcabcabcfff';
	b := '';
	a := copy(b, 1,1);
	write(a)

end.