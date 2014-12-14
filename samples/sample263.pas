{copy test : delka podretezce je vetsi, nez retezce/ Zkopiruje jen retezec, nehodi error}
var	
 	a : string;
	b : string;
	
begin
	a := 'abc';
	b := 'abcabcabcfff';
	b := copy(a, 1,12);
	write(b)

end.