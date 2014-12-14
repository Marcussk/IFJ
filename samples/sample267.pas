{copy test : kopirovani s expr retezce}
var	
 	a : string;
	b : string;
	
begin
	a := 'abcabcabcfff';
	b := 'ab';
	a := copy((b+b+b), 1,length(b)+length(b)+length(b)); {3*b = 'ababab'}
	write(a)

end.