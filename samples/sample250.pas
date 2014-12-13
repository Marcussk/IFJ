{write/ read basic test}
var
	a : integer;
	b : integer; 
	c : integer;
begin
	a := 10;
	b := 15;
	c := a + b;
	write('a je :', a, '       b je :', b, ''#10'');
	write('suma ab je :', c,''#10'', 'wtite new a:');
	readln(a);
	c := a + b;
	write(''#10'','new suma je = ',  c)
end.