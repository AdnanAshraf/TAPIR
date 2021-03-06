function result = generateAudioData( binData )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

TapirConf;

% seperates msg to frames by framesize
blockedMsg = reshape(binData, noDataFrame,[]);
numBlocks = size(blockedMsg, 2);

% txSignal = zeros((symLength+lenPrefix) * numBlocks + guardInterval,1);
result = zeros((symLength+lenPrefix) * numBlocks,1);

%for each block
for idx = 1:numBlocks

    block = blockedMsg(:,idx);
    
	%%%%% DBPSK modulation %%%%%
% 	block = real(dpskmod(block,2));
%     modBlk = block;
    
    block(block == -1) = 0; % For Convolutional Encoding
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    %%%%% Convolutional encoding %%%%%
    block = convenc(block, trel);
%     blockSize = length(block);
    block(block == 0) = -1; % To reduce the dynamic range of output signal.
    convEncBlk = block;
    
    
    %%%%% Interleaver %%%%% 
    block = matintrlv(convEncBlk, intRows, intCols);
%     interleavedBlk = block;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%% Add Pilot & DC %%%%%
%     block = [ block(1:blockSize/2); zeros(addCarrier,1); block(blockSize/2+1:end)];

    
    reshapedBlock = reshape(block, lenBitsBetweenPilot, []);
    firstHalfBlk = reshape( [pilot(1:length(pilot)/2); reshapedBlock(:,1:size(reshapedBlock,1)/2)], [], 1);
    secondHalfBlk = reshape( [reshapedBlock(:,size(reshapedBlock,1)/2 + 1 :end); pilot(length(pilot)/2+1 : end)], [], 1);
    block = [firstHalfBlk ; zeros(noDcCarrier,1) ;secondHalfBlk];
    
%     pilotBlk = block;

%     block = [1; block(1:4); 1; block(5:8); zeros(noDcCarrier,1) ; block(9:12); 1; block(13:16);1];
   
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
    %%%% IFFT %%%%% 
    
    if(mod(noDcCarrier + noPilotCarrier,2) == 0)
        appendZeroLength = (symLength - length(block)) / 2;
        prependZeroLength = appendZeroLength;
    else
        appendZeroLength = (symLength - length(block) - 1) / 2;
        prependZeroLength = appendZeroLength + 1;
    end
    

    block = ifft(fftshift([ zeros(prependZeroLength,1) ;block; zeros(appendZeroLength,1)]), symLength);
    
    block = symLength / sqrt(noTotCarrier) * block;
%     ifftBlock = block;
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	%%%%% Add Cyclic Prefix %%%%%%%
    
    block = [block(end - lenPrefix +1 : end); block];
%     extendedBlk = block;
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%% Add Preamble %%%%%
    
%     block = [ repmat(shortPreamble, noPreambleRepeat, 1); block];
%     preambledBlock = block;

    startIdx = (idx-1) * (txBlockLength) + 1;
    result(startIdx : (startIdx+ txBlockLength - 1)) = block; 
end



end

