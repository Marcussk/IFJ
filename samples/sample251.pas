{unarni minus basic test}
var
	a : integer;
	b : real; 
	
begin
	a := -5;
	write('puvodni a je :', a,''#10'');
	b := 2.2;
	write('puvodni b je :', b,''#10'');
	a := -a;
	write('po un minusu a je :', a,''#10'');
	b := -b;
	write('po un minusu b je :', b,''#10'')
	
end.