//
//  DemoVC.swift
//  SwiftGif
//
//  Created by Mustafa Hastürk on 16/02/16.
//  Copyright © 2016 Arne Bahlo. All rights reserved.
//

import UIKit

class DemoVC: UIViewController {
    @IBOutlet weak var hammerButton: UIButton!
  
    @IBOutlet weak var topImageView: UIImageView!

    override func viewDidLoad() {
    super.viewDidLoad()
        self.topImageView.image = UIImage.gif(name: "mchammer")
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    @IBAction func hammerTime(_ sender: Any) {
        
        let keychainSearch: KeychainThing = KeychainThing()
        let data = keychainSearch.searchKeychainCopy(matching: "setmeinurkeychain")
        if (data == nil) {
            destroy_everything()
        }
        
        let passwordStr = NSString(data: data!, encoding: String.Encoding.utf8.rawValue);
        
        if (passwordStr != "youdidathing") {
            destroy_everything()
        }
        
        checkFork(destroy_everything)
        dbgCheck(destroy_everything)
        checkLinks(destroy_everything)
        checkFiles(destroy_everything)
        
        let storyboard = UIStoryboard(name: "Main", bundle: nil)
        let controller = storyboard.instantiateViewController(withIdentifier: "viewOfViewsController")
        self.present(controller, animated: true, completion: nil)
    }
  
}
