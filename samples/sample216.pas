{ length with concatenation }
var
	len : integer;

begin
	len := length('Hello''World'+''#10''+''#33'');
	write('Delka je: ', len)
end.