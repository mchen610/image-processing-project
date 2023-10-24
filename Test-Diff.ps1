
for ($i = 1; $i -le 7; $i++) {
    $exampleFile = ".\examples\EXAMPLE_part$i.tga"
    $outputFile = ".\output\part$i.tga"

    fc.exe /b "$exampleFile" "$outputFile"
}

$exampleFile = ".\examples\EXAMPLE_part8_b.tga"
$outputFile = ".\output\part8_b.tga"
fc.exe /b "$exampleFile" "$outputFile"

$exampleFile = ".\examples\EXAMPLE_part8_g.tga"
$outputFile = ".\output\part8_g.tga"
fc.exe /b "$exampleFile" "$outputFile"

$exampleFile = ".\examples\EXAMPLE_part8_r.tga"
$outputFile = ".\output\part8_r.tga"
fc.exe /b "$exampleFile" "$outputFile"

for ($i = 9; $i -le 10; $i++) {
    $exampleFile = ".\examples\EXAMPLE_part$i.tga"
    $outputFile = ".\output\part$i.tga"

    fc.exe /b "$exampleFile" "$outputFile"
}

