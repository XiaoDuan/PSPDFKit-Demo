//
//  PSPDFViewControllerDelegate.h
//  PSPDFKit
//
//  Copyright 2011-2012 Peter Steinberger. All rights reserved.
//

#import "PSPDFKitGlobal.h"
#import "PSPDFViewController.h"

@protocol PSPDFAnnotationView;
@class PSPDFDocument, PSPDFPageInfo, PSPDFPageCoordinates, PSPDFAnnotation, PSPDFPageView, PSPDFScrollView;

/// Implement this delegate on your UIViewController to get notified by PSPDFViewController.
@protocol PSPDFViewControllerDelegate <NSObject>

@optional

/* global document handling */

/// Allow/disallow document setting.
/// Can be useful if you e.g. want to block the opening of a different document reference via a outline entry.
- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldSetDocument:(PSPDFDocument *)document;

/// Time to adjust PSPDFViewController before a PSPDFDocument is displayed
- (void)pdfViewController:(PSPDFViewController *)pdfController willDisplayDocument:(PSPDFDocument *)document;

/// Delegate to be notified when pdfController finished loading
- (void)pdfViewController:(PSPDFViewController *)pdfController didDisplayDocument:(PSPDFDocument *)document;

/* Events */

// Note: If you need more scroll events, subclass PSPDFScrollView and relay your custom scroll events. Don't forget calling super though.

/// Control scrolling to pages. Not implementing this will return YES.
- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldScrollToPage:(NSUInteger)page;

/// Controller did show/scrolled to a new page (at least 51% of it is visible)
- (void)pdfViewController:(PSPDFViewController *)pdfController didShowPageView:(PSPDFPageView *)pageView;

/// Page was fully rendered at zoomlevel = 1
- (void)pdfViewController:(PSPDFViewController *)pdfController didRenderPageView:(PSPDFPageView *)pageView;

/// Will be called when viewMode changes
- (void)pdfViewController:(PSPDFViewController *)pdfController didChangeViewMode:(PSPDFViewMode)viewMode;

/** Will be called after page rect has been dragged.
    If decelerate is YES, this will be called again after deceleration is complete.
    velocity/targetContentOffset are only available in iOS5+.
 
    You can also change the target with changing targetContentOffset.
 
    This delegate combines following scrollViewDelegates:
    - scrollViewWillEndDragging (iOS5) / scrollViewDidEndDragging (iOS4)
    - scrollViewDidEndDecelerating

    Note: be careful to not dereference a nil pointer in targetContentOffset.
 */
- (void)pdfViewController:(PSPDFViewController *)pdfController didEndPageDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset;

/// Will be called after zooming animation is complete.
- (void)pdfViewController:(PSPDFViewController *)pdfController didEndPageScrollingAnimation:(UIScrollView *)scrollView;

/// Will be called after zoom level has been changed, either programatically or manually.
- (void)pdfViewController:(PSPDFViewController *)pdfController didEndPageZooming:(UIScrollView *)scrollView atScale:(CGFloat)scale;

/// Return a PSPDFDocument for a relative path.
/// If this is unimplemented, we try to find the PDF ourself with using the current document's basePath.
- (PSPDFDocument *)pdfViewController:(PSPDFViewController *)pdfController documentForRelativePath:(NSString *)relativePath;

/// If user tapped within page bounds, this will notify you.
/// return YES if this touch was processed by you and need no further checking by PSPDFKit.
/// Note: PSPDFPageInfo may has only page=1 if the optimization isAspectRatioEqual is enabled.
/// Note: Before using this, you might consider just adding custom UIView's on top of a PSPDFPageView.
/// Note: Will not send events when thumbnailView is visible.
- (BOOL)pdfViewController:(PSPDFViewController *)pdfController didTapOnPageView:(PSPDFPageView *)pageView info:(PSPDFPageInfo *)pageInfo coordinates:(PSPDFPageCoordinates *)pageCoordinates;

/* annotations */

/// Called before a annotation view is created and added to a page. Defaults to YES if not implemented.
/// if NO is returned, viewForAnnotation will not be called.
- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldDisplayAnnotation:(PSPDFAnnotation *)annotation onPageView:(PSPDFPageView *)pageView;

/// Delegate for tapping on an annotation. If you don't implement this or return false, it will be processed by default action (scroll to page, ask to open Safari)
- (BOOL)pdfViewController:(PSPDFViewController *)pdfController didTapOnAnnotation:(PSPDFAnnotation *)annotation page:(NSUInteger)page info:(PSPDFPageInfo *)pageInfo coordinates:(PSPDFPageCoordinates *)pageCoordinates;

/// Returns a pre-generated annotationView that can be modified before being added to the view.
/// If no generator for a custom annotation is found, annotationView will be nil (as a replacement to viewForAnnotation)
/// To get the targeted rect use [annotation rectForPageRect:pageView.bounds];
- (UIView <PSPDFAnnotationView> *)pdfViewController:(PSPDFViewController *)pdfController annotationView:(UIView <PSPDFAnnotationView> *)annotationView forAnnotation:(PSPDFAnnotation *)annotation onPageView:(PSPDFPageView *)pageView;

/// Allow resolving custom path tokens (Documents, Bundle are automatically resolved; you can add e.g. Book and resolve this here). Will only get called for unknown tokens.
- (NSString *)pdfViewController:(PSPDFViewController *)pdfController resolveCustomAnnotationPathToken:(NSString *)pathToken; // return nil if unknown.

/// Invoked prior to the presentation of the annotation view: use this to configure actions etc
- (void)pdfViewController:(PSPDFViewController *)pdfController willShowAnnotationView:(UIView <PSPDFAnnotationView> *)annotationView onPageView:(PSPDFPageView *)pageView;

/// Invoked after animation used to present the annotation view
- (void)pdfViewController:(PSPDFViewController *)pdfController didShowAnnotationView:(UIView <PSPDFAnnotationView> *)annotationView onPageView:(PSPDFPageView *)pageView;

// detailed control for page loading/unloading

/// Called after pdf page has been loaded and added to the pagingScrollView.
- (void)pdfViewController:(PSPDFViewController *)pdfController didLoadPageView:(PSPDFPageView *)pageView;

/// Called before a pdf page will be unloaded and removed from the pagingScrollView.
- (void)pdfViewController:(PSPDFViewController *)pdfController willUnloadPageView:(PSPDFPageView *)pageView;

/// Called before we show a controller modally or in a popover. Allows last minute modifications.
/// The embeddedInController is either a UINavigationController, a UIPopoverController or nil.
/// viewController is of type id because controller like UIPrntInteractionController are no subclasses of UIViewController.
- (void)pdfViewController:(PSPDFViewController *)pdfController willShowController:(id)viewController embeddedInController:(id)controller animated:(BOOL)animated;

/// Called after the controller has been fully displayed. iOS5 only. Isn't called for UIPopoverController's.
- (void)pdfViewController:(PSPDFViewController *)pdfController didShowController:(id)viewController embeddedInController:(id)controller animated:(BOOL)animated;

/// Return NO to stop the HUD change event.
- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldShowHUD:(BOOL)animated;

/// HUD will be displayed.
- (void)pdfViewController:(PSPDFViewController *)pdfController willShowHUD:(BOOL)animated;
/// HUD was displayed (called after the animation finishes)
- (void)pdfViewController:(PSPDFViewController *)pdfController didShowHUD:(BOOL)animated;

/// Return NO to stop the HUD change event.
- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldHideHUD:(BOOL)animated;

/// HUD will be hidden.
- (void)pdfViewController:(PSPDFViewController *)pdfController willHideHUD:(BOOL)animated;
/// HUD was hidden (called after the animation finishes)
- (void)pdfViewController:(PSPDFViewController *)pdfController didHideHUD:(BOOL)animated;


/* deprecated */

// Deprecated. Allows adding custom annotations. Called on any handler that is not recognized by PSPDFKit.
- (UIView <PSPDFAnnotationView> *)pdfViewController:(PSPDFViewController *)pdfController viewForAnnotation:(PSPDFAnnotation *)annotation onPageView:(PSPDFPageView *)pageView __attribute__((deprecated("Deprecated. Use annotationViewForAnnotation instead.")));

@end
