{ sort with concatenation }
var
	sorted : string;

begin
	sorted := sort('Hello''World'+''#33'');
	write('Sorted string is: ', sorted)
end.