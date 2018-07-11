//
//  Level3ViewController.swift
//  IntroLevels
//
//  Created by Christopher Thompson on 6/8/17.
//  Copyright © 2017 Uber. All rights reserved.
//

import UIKit
import Alamofire

class Level3ViewController: UIViewController {

    @IBOutlet weak var lizardImage: UIImageView!
    @IBOutlet weak var paperImage: UIImageView!
    @IBOutlet weak var rockImage: UIImageView!
    @IBOutlet weak var scissorsImage: UIImageView!
    @IBOutlet weak var spockImage: UIImageView!
    
    var things: [String: String] = [:]
    var serverTrustPolicies: [String: ServerTrustPolicy]?
    var manager: SessionManager?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = "l"
        things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = "c"
        
        _528043()
        _289152()
        _673586()
        _238610()
        _452363()
        _860407()
        _782938()
        _127716()
        _459145()
        _839548()
        _990050()
        _561026()
        _223691()
        _205415()
        _356099()
        _239479()
        _889987()
        _380130()
        _60523()
        _367928()
        _764225()
        _108029()
        _69212()
        _863692()
        _289188()
        _512167()
        _107271()
        _364477()
        _564396()
        _895316()
        _102659()
        _774253()
        _296898()
        _222387()
        _33594()
        _593792()
        _408058()
        _304747()
        _769724()
        _718537()
        _838825()
        _836333()
        _666361()
        _343910()
        _876185()
        _482323()
        _948802()
        _627987()
        _663901()
        _358163()
        _820841()
        _633177()
        _89495()
        _798411()
        _743952()
        
        
        serverTrustPolicies = [
            "*.google.com": .pinCertificates(
                certificates: ServerTrustPolicy.certificates(in: Bundle.main),
                validateCertificateChain: true,
                validateHost: true
            )
        ]
    }
    
    // Creating header key
    func _528043() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.o }
    func _452363() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.o }
    func _860407() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.k }
    func _782938() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.space }
    func _127716() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.a }
    func _459145() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.t }
    func _839548() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.space }
    func _239479() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.m }
    func _889987() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.e }
    func _380130() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.space }
    func _564396() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.i }
    func _895316() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.space }
    func _102659() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.a }
    func _774253() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.m }
    func _769724() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.space }
    func _718537() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.a }
    func _343910() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.space }
    func _876185() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.h }
    func _482323() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.e }
    func _948802() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.a }
    func _627987() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.d }
    func _663901() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.e }
    func _743952() { things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j] = things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]?.r }
    
    // Creating header value
    func _289152() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.A }
    func _673586() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.p }
    func _238610() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.w }
    func _990050() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.N }
    func _561026() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.curly_left }
    func _223691() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?._1 }
    func _205415() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.m }
    func _356099() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.underscore }
    func _60523() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?._1 }
    func _367928() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.n }
    func _764225() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.underscore }
    func _108029() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.u }
    func _69212() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.r }
    func _863692() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.underscore }
    func _289188() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.n }
    func _512167() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?._0 }
    func _107271() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?._0 }
    func _364477() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.t }
    func _296898() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.w }
    func _222387() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.o }
    func _33594() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.r }
    func _593792() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.k }
    func _408058() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.underscore }
    func _304747() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.t }
    func _838825() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.e }
    func _836333() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.r }
    func _666361() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.e }
    func _358163() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?._3 }
    func _820841() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.f }
    func _633177() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.i }
    func _89495() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.k }
    func _798411() { things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i] = things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]?.curly_right }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func lizardTap(_ sender: Any) {
        computerCheat(str: "lizard")
    }
    
    @IBAction func paperTap(_ sender: Any) {
        computerCheat(str: "paper")
    }
    
    @IBAction func rockTap(_ sender: Any) {
        computerCheat(str: "rock")
    }
    
    @IBAction func scissorsTap(_ sender: Any) {
        computerCheat(str: "scissors")
    }
    
    @IBAction func spockTap(_ sender: Any) {
        computerCheat(str: "spock")
        
    }
    
    func computerCheat(str: String) {
        var title = NSLocalizedString("You lost!",comment:"")
        var msg = NSLocalizedString("The computer chose Spock and you chose " + str + ". You are being reported to our servers for being a loser!", comment:"")
        if str == "spock" {
            title = NSLocalizedString("You still lost!",comment:"")
            msg = NSLocalizedString("Even though you chose Spock, the computer's Spock was faster. You are still being reported as a loser!",comment:"")
        }
        
        let alertController = UIAlertController(title: title, message: msg, preferredStyle: .alert)
        let defaultAction = UIAlertAction(title: NSLocalizedString("Ok", comment: ""), style: .default, handler: { (pAlert) in
            self.reportBeingALoser()
        })
        alertController.addAction(defaultAction)
        self.present(alertController, animated: true, completion: nil)
    }
    
    func reportBeingALoser() {
        let configuration = URLSessionConfiguration.default
        configuration.httpAdditionalHeaders = [things["1".h.p.s.q.s.z.a.r.t.y.p.c.x.w.x.u.i.q.j]!: things["q".q.u.i.w.q.a.d.z.x.v.y.q.u.e.u.c.s.q.i]!]
        
        manager = SessionManager(
            configuration: configuration,
            serverTrustPolicyManager: ServerTrustPolicyManager(policies: serverTrustPolicies!)
        )
        
        let dataRequest = manager!.request("https://google.com")
        dataRequest.responseString(completionHandler: {data in
            print("Debug: User has been reported as a loser!")
        })
    }
}
