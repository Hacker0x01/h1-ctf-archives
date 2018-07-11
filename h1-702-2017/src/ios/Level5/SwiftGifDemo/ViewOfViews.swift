//
//  ViewOfViews.swift
//  SwiftGif
//
//  Created by Christopher Thompson on 6/6/17.
//  Copyright © 2017 Arne Bahlo. All rights reserved.
//

import UIKit

class ViewOfViews: UIView {
    var letter = 0
    var width: CGFloat!
    var height: CGFloat!
    
    override init(frame: CGRect) {
        super.init(frame: frame)
    }
    
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
    }
    
    func drawLetter(context: CGContext?, cellLength: CGFloat, x: CGFloat, y: CGFloat, idx: Int) {
        context?.setFillColor(UIColor.black.cgColor)
        for index in 1...15 {
            if (something(Int32(idx), Int32(index)) == 0) { continue }
                
            let x2 = x + (CGFloat(index).truncatingRemainder(dividingBy: 3)) * cellLength
            let y2 = y + floor(CGFloat(index) / 3) * cellLength
            context?.fill(CGRect(x: x2, y: y2, width: cellLength, height: cellLength))
        }
    }

    override func draw(_ rect: CGRect) {
        let viewWidth:CGFloat = self.bounds.width
        print("viewWidth \(viewWidth)")
        
        let viewHight:CGFloat = self.bounds.height
        print("viewHight \(viewHight)")
        
        let cellCount = CGFloat(40)
        let x1:CGFloat = 0.0
        let x2:CGFloat = viewWidth
        
        let y1:CGFloat = 0.0
        let y2:CGFloat = viewHight
        
        let cellLength:CGFloat = min(viewHight, viewWidth) / cellCount
        print("rowHeight \(cellLength)")
        
        let lineWidth:CGFloat = 2.0
        
        let context = UIGraphicsGetCurrentContext()
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        
        let components: [CGFloat] = [0.0, 0.0, 1.0, 1.0]
        let color = CGColor(colorSpace: colorSpace, components: components)
        
        context!.setLineWidth(lineWidth)
        context!.setStrokeColor(color!)
        
        // draw horizontal lines
        var i:CGFloat = 0.0
        repeat {
            
            context?.move(to: CGPoint(x:x1, y:cellLength + i))
            context?.addLine(to: CGPoint(x:x2, y:cellLength + i))
            
            i = i + cellLength
            
        } while i < rect.height - cellLength
        
        // draw vertical lines
        var j:CGFloat = 0.0
        repeat {
            
            context?.move(to: CGPoint(x:cellLength + j, y:y1))
            context?.addLine(to: CGPoint(x:cellLength + j, y:y2))
            
            j = j + cellLength
            
        } while j < rect.width - cellLength
        
        context?.strokePath()
        
        var startX:CGFloat = 13 * cellLength
        var startY:CGFloat = 0
        for  idx in 0...32 {
            drawLetter(context: context, cellLength: cellLength, x: startX, y: startY, idx: idx)
            startY += (5 + 1) * cellLength
            
            if startY > rect.height - (5 * cellLength) {
                startY = 0
                startX += (3 + 1) * cellLength
            }
        }
    }
}
