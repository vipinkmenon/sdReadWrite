fileID = fopen('LENG.BIN','r');
data=fread(fileID);

imageWidth = 256;
imageHeight = 256;
numColor = 1;

newData = uint8(zeros(imageWidth*imageHeight*numColor,1));
l=1;
for i = 1:imageWidth %for i 0 to maxColumn
    for j = 1:imageHeight %for j 0 to maxRow
        for k = 1:numColor
            newData(l+(k-1)*(imageWidth*imageHeight)) = data(imageWidth*(j-1)*numColor+(i-1)*numColor+k);  %newData[k] = imageData[maxColumn*j+i]
        end
        l=l+1;
    end
end

fclose(fileID);

finalData = reshape(newData,[imageHeight,imageWidth,numColor]);
imshow(finalData)
