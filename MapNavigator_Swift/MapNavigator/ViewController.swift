//
//  ViewController.swift
//  MapNavigator
//
//  Created by csair on 2019/5/12.
//  Copyright © 2019年 SCP Foundation. All rights reserved.
//

import UIKit
import MapKit
import CoreLocation

class ViewController: UIViewController, MKMapViewDelegate,CLLocationManagerDelegate{

    @IBOutlet weak var mapView: MKMapView!
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var display: UILabel!
    
    var points:NSMutableArray?
    let locationManager = CLLocationManager()
    var currentLocation: CLLocationCoordinate2D?
    var lastLocation: CLLocationCoordinate2D?
    var routeLine: MKPolyline?
    var isStart:Bool=false
    var startAnnotation = MKPointAnnotation()
    var finishAnnotation = MKPointAnnotation()
    var distance = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if CLLocationManager.authorizationStatus() == CLAuthorizationStatus.notDetermined {
            locationManager.requestAlwaysAuthorization()
        }
        
        //self.currentLocationButton.addTarget(self, action: Selector("currentLocationButtonAction"), for: //UIControl.Event.touchUpInside)
        
        self.mapView.mapType = MKMapType.standard
        self.mapView.userTrackingMode = MKUserTrackingMode.follow
        self.mapView.showsUserLocation = true
        self.mapView.delegate = self
        
        self.startAnnotation.title="Start Point"
        self.startAnnotation.subtitle="The point where you start recording"
        self.finishAnnotation.title="End Point"
        self.finishAnnotation.subtitle="The point where you stop recording"
    }
    
    
    @IBAction func startRecord(_ sender: UIButton) {
        if self.currentLocation != nil {
            let span = MKCoordinateSpan(latitudeDelta: 0.01, longitudeDelta: 0.01)
            var region = MKCoordinateRegion(center: self.currentLocation!, span: span)
            region.center = self.currentLocation!
            self.mapView.setRegion(region, animated: true)
        }
        if !isStart {
            self.isStart=true
            titleLabel.text="Recording..."
            let point = CLLocation(latitude: (self.currentLocation?.latitude)!, longitude: (self.currentLocation?.longitude)!)
            if self.points==nil {
                self.points = NSMutableArray()
            }
            else {
                self.points!.removeAllObjects()
            }
            self.points?.add(point)
            
            if self.routeLine != nil {
                self.mapView.removeOverlays(self.mapView.overlays)
            }
            
            self.mapView.removeAnnotation(self.startAnnotation)
            self.mapView.removeAnnotation(self.finishAnnotation)
            self.startAnnotation.coordinate = self.currentLocation!
            self.mapView.addAnnotation(self.startAnnotation)
            
            self.distance=0
            self.display.text="\(distance) Km"
            self.lastLocation=self.currentLocation
        }
        else {
            let ac = UIAlertController(title: "Warning", message: "You have already started recording", preferredStyle: .alert)
            let ok = UIAlertAction(title: "Ok", style: .cancel, handler: nil)
            ac.addAction(ok)
            self.present(ac, animated: true, completion: nil)
        }
    }
    
    @IBAction func stopRecord(_ sender: UIButton) {
        if isStart {
            if self.currentLocation != nil {
                let span = MKCoordinateSpan(latitudeDelta: 0.01, longitudeDelta: 0.01)
                var region = MKCoordinateRegion(center: self.currentLocation!, span: span)
                region.center = self.currentLocation!
                self.mapView.setRegion(region, animated: true)
            }
            self.isStart=false
            titleLabel.text="Recording finished, click 'Draw' to draw the route"
            self.finishAnnotation.coordinate = self.currentLocation!
            self.mapView.addAnnotation(self.finishAnnotation)
        }
        else {
            let ac = UIAlertController(title: "Warning", message: "You are not in recording", preferredStyle: .alert)
            let ok = UIAlertAction(title: "Ok", style: .cancel, handler: nil)
            ac.addAction(ok)
            self.present(ac, animated: true, completion: nil)
        }
    }
    
    @IBAction func startDraw(_ sender: UIButton) {
        if isStart {
            let ac = UIAlertController(title: "Warning", message: "Recording has not finished, click 'Stop' first", preferredStyle: .alert)
            let ok = UIAlertAction(title: "Ok", style: .cancel, handler: nil)
            ac.addAction(ok)
            self.present(ac, animated: true, completion: nil)
        }
        else {
            if self.points != nil {
                self.routeLine = self.polyline()
                
                if self.routeLine != nil {
                    self.mapView.addOverlay(self.routeLine!)
                }
                titleLabel.text="Drawing route successfully"
            }
            else {
                titleLabel.text="No record available, click 'Start' to start recording"
            }
        }
    }
    
    func mapView(_ mapView: MKMapView, didUpdate userLocation: MKUserLocation) {
        self.currentLocation = CLLocationCoordinate2DMake(userLocation.coordinate.latitude, userLocation.coordinate.longitude)
        let point = CLLocation(latitude: userLocation.coordinate.latitude, longitude: userLocation.coordinate.longitude)
        if isStart {
            if self.points == nil {
                self.points = NSMutableArray()
            }
            self.points?.add(point)
            let dis:CLLocationDistance = CLLocation(latitude: (currentLocation?.latitude)!, longitude: (currentLocation?.longitude)!).distance(from: CLLocation(latitude: (lastLocation?.latitude)!, longitude: (lastLocation?.longitude)!))
            self.distance+=Int(dis)
            self.display.text="\(Double(distance)/1000) Km"
            self.lastLocation=self.currentLocation
        }
        
        
       /* if self.routeLine != nil {
            self.mapView.removeOverlays(self.mapView.overlays)
        }
        
        self.routeLine = self.polyline()
        
        if self.routeLine != nil {
            self.mapView.addOverlay(self.routeLine!)
        }*/
        
    }
    
    func mapView(_ mapView: MKMapView, rendererFor overlay: MKOverlay) -> MKOverlayRenderer {
        let polylineRenderer = MKPolylineRenderer(polyline: self.routeLine!)
        polylineRenderer.strokeColor = UIColor.blue
        polylineRenderer.lineWidth = 4
        return polylineRenderer
    }

    
    func polyline() -> MKPolyline {
        
        var coords = [CLLocationCoordinate2D]() // 有类型的 Array 变量
        
        for i in 0..<self.points!.count {
            let location = self.points?.object(at: i) as! CLLocation
            let c = location.coordinate
            coords.append(c)
            
        }
        return MKPolyline(coordinates: &coords, count: self.points!.count)
    }


}

