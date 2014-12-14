{sort test : razeni retezce s konkatenaci ve forme expr}
var	
 	a : string;

	
begin
	a := 'abab';
	a := sort((a+a));
	write(a)

end.